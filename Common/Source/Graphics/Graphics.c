#include "Graphics.h"
#include "../Time/Time.h"
#include "../Helpers/Helpers.h"
#include "../Globals/Globals.h"
#include "../Texture/Texture.h"
#include "../ShaderProgram/ShaderProgram.h"
#include "../VertexFormat/VertexFormat.h"
#include "../Model/Model.h"
#include "../LodePNG/lodepng.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//TODO: REFACTOR ME
#ifdef _BUILD_ANDROID
	#include <android/log.h>
	#define APPNAME "com.joecameron.game"

#endif

#ifdef _BUILD_ANDROID
	//POSSIBLE FIX
	#include "../Includes/LinMath/linmath.h"


#else
	//ORIGINAL. TODO: SPECIFIY PLATFORMS
	//#include <LinMath\linmath.h>
	#include "../Includes/LinMath/linmath.h"

#endif

//Get OpenGL header
#if defined(_BUILD_WIN32) || defined(_BUILD_MAC) || defined(_BUILD_LINUX)
    //#include <GL\glew.h>
	#include "../Includes/GL/glew.h"
  
#elif defined (_BUILD_ANDROID)
    #include <GLES2/gl2.h>
    
#endif

//*****************
// Static variables
//*****************
static GLuint instanceCount = 0;

static GLfloat clearColor[] = {0.535f,0.535f,0.8f,1.0f};

//Hashtables
struct ShaderProgram* graphicsPrograms = NULL;
struct Texture*       textures         = NULL;
struct Model*         models           = NULL;

//Some super stuff TODO: refactor!!!
mat4x4 modelMatrix;//Model pos/rot/scale (MODEL -> WORLD)
mat4x4 viewMatrix;//Camera pos/rotation (WORLD -> VIEW)
mat4x4 projectionMatrix;//Perspective, viewport size (VIEW -> PROJECTION)

//*****************************
// Static function declarations
//*****************************
//init funcs
static void init(const Graphics*);
static void initShaders(void);
static void initTextures(void);
static void initModels(void);

//load funcs
static void loadShader(char* aShaderProgramName);
static void loadTexture(const char* filename, GLint repeatmode, GLint magfilter);
static void loadModel(const char* filename); //TODO: implement

//Error handling funcs
static void  printfGLErrors();
static void  clearGLErrors();

//Shader compilation funcs
static char* getShaderSource(char* aShaderProgram, char* id);
static uint  checkShaderCompilerErrors(const GLint shader, const char* shaderName, const char* shaderType);

//Draw funcs
static void  draw(const Graphics*);
static void  testDraw(void); //TODO: refactor

static void createQuadModel(); //TODO: consider future of hardcoded models. Do certain ones make sense?
static void createCubeModel();

//ShaderProgram hashtable functions
static void add_ShaderProgram(char* shaderProgramName, GLuint handle) ;
struct ShaderProgram* ShaderProgram_find(char* shaderProgram_Name);

//Texture hashtable functions
static void     add_Texture(char* textureName, GLuint handle);
struct Texture* find_Texture(char* texture_Name);

//Model hashtable functions
static void add_Model(char* modelName, GLuint vbo, GLuint ibo, GLuint indexLength);
struct Model* find_Model(char* model_Name);

//******************************
// External function definitions
//******************************
Graphics* Graphics_New(void)
{
    Graphics* output = 0;

    if (instanceCount == 0)
    {
        //Create instance
        output = (Graphics*)malloc (sizeof (Graphics));

        //Initialize instance
        output->init = init ;
        output->draw = draw ;

        instanceCount++;

    }

    return output;

}

void Graphics_Free(const Graphics* aGraphics)
{
    //Deallocate members
    free(aGraphics->init);
    free(aGraphics->draw);

    //Deallocate object
    free((void*)aGraphics);

    instanceCount--;

}



//****************************
// static function definitions
//****************************
static void init  (const Graphics* self)
{
    #ifdef _DEBUG
        printf("Graphics.init()\n");
        printf("context renderer string: \"%s\"\n", glGetString(GL_RENDERER));
        printf("context vendor string: \"%s\"\n", glGetString(GL_VENDOR));
        printf("version string: \"%s\"\n", glGetString(GL_VERSION));
    
    #endif
    
    clearGLErrors();
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(clearColor[0],clearColor[1],clearColor[2],clearColor[3]);
    
    ///////////////////////////////////////////////////////////////////////////////////DELETE ME//////////
    ////THISISANANDROIDIOTEST
	//printf("Starting a test\n");
    //{
    //	FILE *fp;
    //	char* str = "string";
    //	int x = 10;
	//
    //	fp=fopen("/sdcard/test.txt", "w");
    //	if(fp == NULL)
    //	    exit(-1);
    //	fprintf(fp, "This is a test of writing a file at sdcard test\n");
    //	fprintf(fp, "The string has %d words and keyword %s\n", x, str);
    //	fclose(fp);
	//
	//
    //}


    //Initialize assets
    initShaders();
    initTextures();
    initModels();
    
	#ifdef _BUILD_ANDROID
    //Test model good
    if (find_Model("Cube") != 0)
    {

    	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Cube exists", 1);

    }
    else
    	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Cube doesnt exist", 1);

    //Test shader
    if (ShaderProgram_find("AlphaCutOff") != 0)
    {
    	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "alphacutoff shader exists", 1);

    }
    else
    {
    	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "alphacutoff shader doesnt exist", 1);

    }

    //Test texture
    if (find_Texture("awesome.png") != 0)
    {
    	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "awesome texture exists", 1);

    }
    else
    {
    	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "awesome texture doesnt exist", 1);

    }
	#endif

    printf("Graphics initialization complete\n***************************************************************\n");

}

static void  initShaders(void) //TODO: at least automate this function
{
    printf("Graphics.initShaders()\n");

    loadShader("AlphaCutOff");

    //TODO: load shader directory

}

static void  initTextures(void) //TODO: at least automate this function
{
    printf("Graphics.initTextures()\n");

    loadTexture("awesome.png", GL_REPEAT, GL_NEAREST);
    loadTexture("brick.png", GL_REPEAT, GL_NEAREST);

    //TODO: load texture directory

}

static void loadShader(char* aShaderProgramName) 
{
    GLuint vertexShader  = 0;
    GLuint fragShader    = 0;
    GLuint shaderProgram = 0;
    
    char* vertexShaderSource   = 0;
    char* fragmentShaderSource = 0;
    
    //Create two empty shaders for Vertex/Frag program
    vertexShader = glCreateShader( GL_VERTEX_SHADER   );
    fragShader   = glCreateShader( GL_FRAGMENT_SHADER );

    //Retrieve shader source code
    vertexShaderSource   = getShaderSource(aShaderProgramName,"Vertex");
    fragmentShaderSource = getShaderSource(aShaderProgramName,"Fragment");

    #ifdef _DEBUG
        printf("Compiling %s shader program...\n",aShaderProgramName);
                
    #endif

    //Compile the shaders
    glShaderSource(vertexShader, 1, &vertexShaderSource  , 0);
    glCompileShader(vertexShader);
    glShaderSource(fragShader  , 1, &fragmentShaderSource, 0);
    glCompileShader(fragShader);

    //Check for compilation errors, print compiler log
    checkShaderCompilerErrors(vertexShader,aShaderProgramName,"Vertex");
    checkShaderCompilerErrors(fragShader,aShaderProgramName,"Fragment");

    //Create the shader program & compile shaders into graphics programs  
    shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragShader  );
    glLinkProgram(shaderProgram);

    //TODO: ADD PROGRAM TO HASHTABLE
    //TODO: ADD CHAR* NAME FOR RETRIVAL FROM HASHTABLE
    {
        //struct ShaderProgram* graphicsProgram = (struct ShaderProgram*)malloc(sizeof(ShaderProgram)*1);
        //{
        //    graphicsProgram->handle = shaderProgram;
        //    graphicsProgram->name   = aShaderProgramName;
        //
        //}
        //add_ShaderProgram(graphicsProgram);
        add_ShaderProgram(aShaderProgramName,shaderProgram);

    }

    printfGLErrors();

    return;
    
}

static void draw(const Graphics* self)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    testDraw();

}

static char* getShaderSource(char* aShaderProgram, char* id)
{
    //GLSL library data
    static short shaderLibraryWasLoaded      = 0;
    static char* shaderLibraryGlobalSource   = 0;
    static char* shaderLibraryVertexSource   = 0;
    static char* shaderLibraryFragmentSource = 0;

    char* output = 0;
    {
        FILE *ptr_file = 0;

        //Load shader library if it hasnt already been done
        if (shaderLibraryWasLoaded == 0)
        {
            shaderLibraryWasLoaded = 1;
            {
                char* shaderLibraryFileBuffer = 0;
                {
                    #ifdef _DEBUG
                        printf("Loading GLSLLibrary to memory...\n");
                
                    #endif

                    shaderLibraryFileBuffer = textFileToCString(cStringAdd2(_SHADER_DIR,"GLSLLibrary.html"));

                    if (shaderLibraryFileBuffer == 0)
                    {
                        #ifdef _DEBUG
                            printf("ERROR: Shader library source file is missing\n");

                        #endif

                    }
                    else
                    {
                        shaderLibraryGlobalSource   = retrieveXMLElementById(shaderLibraryFileBuffer,"Global"  );
                        shaderLibraryVertexSource   = retrieveXMLElementById(shaderLibraryFileBuffer,"Vertex"  );
                        shaderLibraryFragmentSource = retrieveXMLElementById(shaderLibraryFileBuffer,"Fragment");
                        
                    }

                }
                
                free(shaderLibraryFileBuffer);

            }

        }

        //Load shader source file
        {
            char* shaderProgramFileBuffer = 0;
            {
                char* sourceFilePath = cStringAdd3(_SHADER_DIR,aShaderProgram,".html");
                {
                    #ifdef _DEBUG
                        printf("Loading %s.%s to memory...\n",aShaderProgram,id);

                    #endif

                    shaderProgramFileBuffer = textFileToCString(sourceFilePath);
                
                }

                free(sourceFilePath);
                

                if (shaderProgramFileBuffer == 0)
                {
                    #ifdef _DEBUG
                    printf("ERROR: %s.%s source file is missing\n",aShaderProgram,id);
                
                    #endif

                }
                else
                {
                    uint len = 0;
                    char* typeDependantInclude = 0;
                    char* sourceRaw = retrieveXMLElementById(shaderProgramFileBuffer,id);

                    if (id == "Vertex")
                        typeDependantInclude = shaderLibraryVertexSource;
                    else if (id == "Fragment")
                        typeDependantInclude = shaderLibraryFragmentSource;

                    //if (typeDependantInclude == 0)
                        output = cStringAdd2(shaderLibraryGlobalSource,sourceRaw);
                    //else
                    //    output = cStringAdd3(shaderLibraryGlobalSource,typeDependantInclude,sourceRaw);

                }
        
            }
        
            free(shaderProgramFileBuffer);
        
        }

    }
    return output;

}

static void  printfGLErrors()
{
    GLenum errorCode = glGetError();

    for(;;)
    {
        if (errorCode == 0)
            break;
        else
            printf("WARNING: GLerror %d\n",errorCode);

        errorCode = glGetError();

    }

}

//
// GLFW creates bad value errors. This cleans up errors before this code takes control of the gl context
//
static void clearGLErrors()
{
    while((glGetError()) != 0)
        glGetError();

}

//
// Return 0 if failed, 1 if success
//
static uint  checkShaderCompilerErrors(const GLint shader, const char* shaderName, const char* shaderType)
{
    GLint infoLogLength = 0;
    GLchar* strInfoLog  = 0;
    GLint compileStatus = 0;
    
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    
    strInfoLog = (GLchar*)malloc(sizeof(GLchar) * (infoLogLength + 1));
    
    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
    
    printf("********\n%s.%s's compiler log:\n********\n%s\n", shaderName, shaderType, strInfoLog);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    
    if (compileStatus == 0)
    //    printf("%s.%s failed to compile. Please refer to compiler log above",shaderName,shaderType);
    
    return compileStatus;
}

//Hashtable functions
void add_ShaderProgram(char* shaderProgramName, GLuint handle) //DONE?
{
    struct ShaderProgram* entry;

    entry = (struct ShaderProgram*)malloc(sizeof(struct ShaderProgram));
    //strcpy(entry->name,shaderProgramName);
    entry->name = shaderProgramName;
    entry->handle = handle;

    HASH_ADD_INT( graphicsPrograms, name, entry );    

}

void add_Texture(char* textureName, GLuint handle) //DONE?
{
    struct Texture* entry;

    entry = (struct Texture*)malloc(sizeof(struct Texture));
    //strcpy(entry->name,textureName);
    entry->name = textureName;
    entry->handle = handle;

    HASH_ADD_INT( textures, name, entry );

}

void add_Model(char* modelName, GLuint vbo, GLuint ibo, GLuint indexLength) //TODO: confirm?
{
    struct Model* entry;

    entry = (struct Model*)malloc(sizeof(struct Model));
    entry->name        = modelName;
    entry->VBO         = vbo;
    entry->IBO         = ibo;
    entry->indexLength = indexLength;

    HASH_ADD_INT( models, name, entry );

}

struct ShaderProgram* ShaderProgram_find(char* shaderProgram_Name) 
{
    struct ShaderProgram* output = 0;
    {
        HASH_FIND_INT( graphicsPrograms, &shaderProgram_Name, output );

        //TODO: add default value
        //if (output == 0)
        //    output = 

    }

    return output;

}

struct Texture* find_Texture(char* texture_Name)
{
    struct Texture* output = 0;
    {
        HASH_FIND_INT( textures, &texture_Name, output );

    }

    return output;

}

struct Model* find_Model(char* model_Name)
{
    struct Model* output = 0;
    {
        HASH_FIND_INT( models, &model_Name, output );

    }

    return output;

}

//
// Generate and store quad vertex data
//
static void createQuadModel()
{
    struct Model quadModel;
    glGenBuffers(1,&quadModel.VBO);

    #ifdef _DEBUG
        printf("Generating Quad...\n");

    #endif

    //Create buffer data
    glBindBuffer(GL_ARRAY_BUFFER,quadModel.VBO);
    {
        GLfloat size = 1.0f;

        GLfloat vertexData[6 * 8] =//6 verts, 8 attributes vertexformat
        {
            //           x,               y,    z,   u,   v,  Nx,  Ny,  Nz,
            size -(size/2),  size -(size/2),  0.0, 1.0, 0.0, 0.0, 0.0, 1.0, // 1--0
            0.0  -(size/2),  size -(size/2),  0.0, 0.0, 0.0, 0.0, 0.0, 1.0, // | /
            0.0  -(size/2),  0.0  -(size/2),  0.0, 0.0, 1.0, 0.0, 0.0, 1.0, // 2

            size -(size/2),  size -(size/2),  0.0, 1.0, 0.0, 0.0, 0.0, 1.0, //    0
            0.0  -(size/2),  0.0  -(size/2),  0.0, 0.0, 1.0, 0.0, 0.0, 1.0, //  / |
            size -(size/2),  0.0  -(size/2),  0.0, 1.0, 1.0, 0.0, 0.0, 1.0, // 1--2

        };

        quadModel.indexLength = 6; //6 vertex formats

        glBufferData(GL_ARRAY_BUFFER,sizeof(VertexFormat)*quadModel.indexLength,vertexData,GL_STATIC_DRAW);
        add_Model("Quad",quadModel.VBO,quadModel.IBO,quadModel.indexLength);

    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

static void createCubeModel()
{
    struct Model cubeModel;
    glGenBuffers(1,&cubeModel.VBO);

    #ifdef _DEBUG
        printf("Generating Cube...\n");

    #endif

    //Create buffer data
    glBindBuffer(GL_ARRAY_BUFFER,cubeModel.VBO);
    {
        GLfloat size = 1.0f;

        GLfloat vertexData[36 * 8] =//36 verts, 8 attributes vertexformat
        {
            //           x,               y,        z,   u,   v,  Nx,  Ny,   Nz,
            size -(size/2),  size -(size/2),  -size/2, 0.0, 0.0, 0.0, 0.0, -1.0, // 2--0
            0.0  -(size/2),  0.0  -(size/2),  -size/2, 1.0, 1.0, 0.0, 0.0, -1.0, // | /
            0.0  -(size/2),  size -(size/2),  -size/2, 1.0, 0.0, 0.0, 0.0, -1.0, // 1

            size -(size/2),  size -(size/2),  -size/2, 0.0, 0.0, 0.0, 0.0, -1.0, //    0
            size -(size/2),  0.0  -(size/2),  -size/2, 0.0, 1.0, 0.0, 0.0, -1.0, //  / |
            0.0  -(size/2),  0.0  -(size/2),  -size/2, 1.0, 1.0, 0.0, 0.0, -1.0, // 2--1

            //South
            //           x,               y,        z,   u,   v,   Nx,  Ny,   Nz,
            size -(size/2),  size -(size/2),   size/2, 1.0, 0.0,  0.0, 0.0, +1.0, // 1--0
            0.0  -(size/2),  size -(size/2),   size/2, 0.0, 0.0,  0.0, 0.0, +1.0, // | /
            0.0  -(size/2),  0.0  -(size/2),   size/2, 0.0, 1.0,  0.0, 0.0, +1.0, // 2

            size -(size/2),  size -(size/2),   size/2, 1.0, 0.0,  0.0, 0.0, +1.0, //    0
            0.0  -(size/2),  0.0  -(size/2),   size/2, 0.0, 1.0,  0.0, 0.0, +1.0, //  / |
            size -(size/2),  0.0  -(size/2),   size/2, 1.0, 1.0,  0.0, 0.0, +1.0, // 1--2

            //West
            //           x,              y,         z,   u,   v,   Nx,  Ny,  Nz,
            0.0  -(size/2), size -(size/2),    size/2, 1.0, 0.0, -1.0, 0.0, 0.0, // 2--0
            0.0  -(size/2), size -(size/2),   -size/2, 0.0, 0.0, -1.0, 0.0, 0.0, // | /
            0.0  -(size/2), 0.0  -(size/2),   -size/2, 0.0, 1.0, -1.0, 0.0, 0.0, // 1

            0.0 -(size/2),  size -(size/2),    size/2, 1.0, 0.0, -1.0, 0.0, 0.0, //    0
            0.0 -(size/2),  0.0  -(size/2),   -size/2, 0.0, 1.0, -1.0, 0.0, 0.0, //  / |
            0.0 -(size/2),  0.0  -(size/2),    size/2, 1.0, 1.0, -1.0, 0.0, 0.0, // 2--1

            //East
            //           x,               y,        z,   u,   v,   Nx,   Ny,  Nz,
            size -(size/2),  size -(size/2),   size/2, 0.0, 0.0, +1.0,  0.0, 0.0, // 2--0
            size -(size/2),  0.0  -(size/2),  -size/2, 1.0, 1.0, +1.0,  0.0, 0.0, // | /
            size -(size/2),  size -(size/2),  -size/2, 1.0, 0.0, +1.0,  0.0, 0.0, // 1

            size -(size/2),  size -(size/2),   size/2, 0.0, 0.0, +1.0,  0.0, 0.0, //    0
            size -(size/2),  0.0  -(size/2),   size/2, 0.0, 1.0, +1.0,  0.0, 0.0, //  / |
            size -(size/2),  0.0  -(size/2),  -size/2, 1.0, 1.0, +1.0,  0.0, 0.0, // 2--1

            //Down
            //           x,              y,         z,   u,   v,   Nx,  Ny,  Nz,
            size -(size/2),  0.0 -(size/2),   -size/2, 1.0, 0.0,  0.0, -1.0, 0.0, // 2--0
            0.0  -(size/2),  0.0 -(size/2),    size/2, 0.0, 1.0,  0.0, -1.0, 0.0, // | /
            0.0  -(size/2),  0.0 -(size/2),   -size/2, 0.0, 0.0,  0.0, -1.0, 0.0, // 1

            size -(size/2),  0.0  -(size/2),  -size/2, 1.0, 0.0,  0.0, -1.0, 0.0, //    0
            size -(size/2),  0.0  -(size/2),   size/2, 1.0, 1.0,  0.0, -1.0, 0.0, //  / |
            0.0  -(size/2),  0.0  -(size/2),   size/2, 0.0, 1.0,  0.0, -1.0, 0.0, // 2--1

            //Up
            //           x,               y,       z,   u,   v,    Nx,   Ny,  Nz,
            size -(size/2),  1.0  -(size/2), -size/2, 1.0, 0.0,   0.0, +1.0, 0.0, // 1--0
            0.0  -(size/2),  1.0  -(size/2), -size/2, 0.0, 0.0,   0.0, +1.0, 0.0, // | /
            0.0  -(size/2),  1.0  -(size/2),  size/2, 0.0, 1.0,   0.0, +1.0, 0.0, // 2

            size -(size/2),  1.0  -(size/2), -size/2, 1.0, 0.0,   0.0, +1.0, 0.0, //    0
            0.0  -(size/2),  1.0  -(size/2),  size/2, 0.0, 1.0,   0.0, +1.0, 0.0, //  / |
            size -(size/2),  1.0  -(size/2),  size/2, 1.0, 1.0,   0.0, +1.0, 0.0, // 1--2

        };

        cubeModel.indexLength = 36; //6 vertex formats

        glBufferData(GL_ARRAY_BUFFER,sizeof(VertexFormat)*cubeModel.indexLength,vertexData,GL_STATIC_DRAW);
        add_Model("Cube",cubeModel.VBO,cubeModel.IBO,cubeModel.indexLength);

    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

static void testDraw(void)
{
    //*************
    // Marshal data
    //*************
    //static GLfloat test = 0;
    //GLfloat time = test++; //TODO: get time

    //Current camera
    //var viewMatrix       = GRAPHICS.getActiveCamera().getViewMatrix();//This should be part of a camera
    //var projectionMatrix = GRAPHICS.getActiveCamera().getProjectionMatrix();//this should be part of a camera

    //This gameobject
    //GLuint vertexArray   = quadModel.VBO;
    struct Model* model = find_Model("Cube");

    GLuint shaderProgram = ShaderProgram_find("AlphaCutOff")->handle;

    ////Model Scale
    //var modelScale = this.getGameObject().getTransform().getLocalScaleMatrix();
    ////Model Rotation
    //var rotation   = this.getGameObject().getTransform().getLocalRotationMatrix();
    ////This transform
    //var object2WorldMatrix = this.getGameObject().getTransform().getWorldMatrix();
    ////Camera pos
    //var cameraPosition = GRAPHICS.getActiveCamera().getGameObject().getTransform().getPosition();

    //setup culling
    glEnable  (GL_DEPTH_TEST);
    glEnable  (GL_CULL_FACE );
    glDisable (GL_CULL_FACE );
    glCullFace(GL_BACK      );

    //************
    // Prep shader
    //************
    glUseProgram(shaderProgram);

    {
        //***************************
        // Pass vertex data to shader
        //***************************
        //Get attribute locations
        GLint attPos;
        GLint attUV;
        GLint attNorm;

        attPos = glGetAttribLocation( shaderProgram, "a_Pos" );
        glEnableVertexAttribArray(attPos);

        attUV = glGetAttribLocation( shaderProgram, "a_UV" );
        glEnableVertexAttribArray(attUV);

        attNorm = glGetAttribLocation( shaderProgram, "a_Normal" );
        glEnableVertexAttribArray(attNorm);

        glBindBuffer( GL_ARRAY_BUFFER, model->VBO);

        //Create vertex attribute pointers..
        //Position attribute pointer
        glVertexAttribPointer
        (
            attPos, //Position attribute index
            3, //Pos size
            GL_FLOAT,
            GL_FALSE, 
            sizeof(GLfloat)*(3+2+3), //stride is size of vertex format in bytes. 4 is float size, 3 pos, 2 uv 
            0 
        
        );

        //UV attribute pointer
        glVertexAttribPointer
        (
            attUV, //Texcoord attribute index
            2, //UV size
            GL_FLOAT,
            GL_FALSE, 
            sizeof(GLfloat)*(3+2+3), //stride is size of vertex format in bytes. 4 is float size, 3 pos, 2 uv 
            (void*)(sizeof(GLfloat)*3) 
        
        );

        //Normal attribute pointer
        glVertexAttribPointer
        (
            attNorm, //Normal attribute index
            3, //Normal size
            GL_FLOAT,
            GL_FALSE, 
            sizeof(GLfloat)*(3+2+3), //stride is size of vertex format in bytes. 4 is float size, 3 pos, 2 uv 
            (void*)(sizeof(GLfloat)*(3+2)) 
        
        );


    }

    //*********************
    // Pass in uniform data
    //*********************
    //Time uniform
    {

        float val = TIME->time()*50;

        GLint uTime = glGetUniformLocation(shaderProgram,"_Time");
        if (uTime != -1)
            glUniform1f(uTime, val );

    }

    //Texture uniform
    {

        GLuint val = find_Texture("awesome.png")->handle;
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture  (GL_TEXTURE_2D, val);
        glUniform1i    (glGetUniformLocation(shaderProgram, "_Texture"), 0);

    }

    /*
    //ModelScale uniform
    var uModelScaleMatrix = glContext.getUniformLocation(shaderProgram,"_ModelScaleMatrix");
    if (uModelScaleMatrix != -1)
        glContext.uniformMatrix4fv(uModelScaleMatrix, false, modelScale);
    
    //ModelRotation uniform
    var uModelRotationMatrix = glContext.getUniformLocation(shaderProgram,"_ModelRotationMatrix");
    if (uModelRotationMatrix != -1)
        glContext.uniformMatrix4fv(uModelRotationMatrix, false, rotation);
        
    //Model translation matrix uniform
    var uObject2WorldMatrix = glContext.getUniformLocation(shaderProgram,"_Object2WorldMatrix");
    if (uObject2WorldMatrix != -1)
        glContext.uniformMatrix4fv(uObject2WorldMatrix, false, object2WorldMatrix);
        
    //View matrix uniform
    var uViewMatrix = glContext.getUniformLocation(shaderProgram,"_ViewMatrix");
    if (uViewMatrix != -1)
        glContext.uniformMatrix4fv(uViewMatrix, false, viewMatrix);
        
    */

    //Model matrix uniform
    {

        GLuint uModelMatrix = glGetUniformLocation(shaderProgram,"_ModelMatrix");
        mat4x4_identity(modelMatrix);//zero out mat
        mat4x4_translate(modelMatrix,0,0,-2);
        
        if (uModelMatrix != -1)
            glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, (GLfloat*)modelMatrix);

    }

    //View matrix uniform
    {

        GLuint uViewMatrix = glGetUniformLocation(shaderProgram,"_ViewMatrix");
        mat4x4_identity(viewMatrix);//zero out mat


        if (uViewMatrix != -1)
            glUniformMatrix4fv(uViewMatrix, 1, GL_FALSE, (GLfloat*)viewMatrix);

    }

    //Projection matrix uniform
    {

        GLuint uProjectionMatrix = glGetUniformLocation(shaderProgram,"_ProjectionMatrix");
        mat4x4_identity(projectionMatrix);//zero out mat
        //???//apply viewport scale
        mat4x4_perspective(projectionMatrix,0.45*3.1415,(5.0/3.0),0.1,10);//apply perspective
        if (uProjectionMatrix != -1)
            glUniformMatrix4fv(uProjectionMatrix, 1, GL_FALSE, (GLfloat*)projectionMatrix);

    }

    
    /*
	//Camera world position uniform
	var uCameraWorldPosition = glContext.getUniformLocation(shaderProgram,"_CameraPos");
	if (uCameraWorldPosition != -1)
		glContext.uniform3fv(uCameraWorldPosition,cameraPosition);

    

    */

    //*****
    // draw
    //*****
    glDrawArrays( GL_TRIANGLES, 0, model->indexLength );//glDrawArrays

}

void loadTexture(const char* filename, GLint repeatmode, GLint magfilter)
{
    #ifdef _DEBUG    
    printf("Loading %s...\n",filename);
    
    #endif
    {
        unsigned char* pngbuffer;
        unsigned int   width, height;
        GLuint texturehandle;

        unsigned int error = lodepng_decode32_file( &pngbuffer, &width, &height, cStringAdd2(_TEXTURE_DIR,filename) );
        
        if(error != 0)
        {
            #ifdef _DEBUG
                printf("Failed to load %s\n",filename);

            #endif

        }
        else
        {
            glGenTextures( 1, &texturehandle );
            glActiveTexture( GL_TEXTURE0 );
            glBindTexture( GL_TEXTURE_2D, texturehandle );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pngbuffer );
            
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatmode );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatmode );
            
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            
            add_Texture(filename,texturehandle);

        }

        free( pngbuffer );

    }
    
}

static void initModels(void)
{
    printf("Graphics.initModels()\n");
    createQuadModel();
    createCubeModel();

    //TODO: load model directory

}
