/*
Copyright (c) 2013, Shodruky Rhyammer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

  Neither the name of the copyright holders nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <unistd.h>
#include <stdint.h>


#define GLS_TMP_BUFFER_SIZE 2097152
#define GLS_DATA_SIZE 356
#define GLS_STRING_SIZE_PLUS 10240 // 256
#define GLS_STRING_SIZE 10160 // 254
#define GLS_ALIGNMENT_BITS 3


// To prevent incompatible version, change every
#define GLS_VERSION 9
// #define GL_MAJOR_VERSION 1
// #define GL_MINOR_VERSION 2


enum GL_Server_Command
{
  GLSC_BREAK,
  GLSC_FLIP,
  GLSC_SEND_DATA,
  GLSC_FLUSH,
  GLSC_get_context,
  
  // EGL commands
  GLSC_eglBindAPI,
  GLSC_eglBindTexImage,
  GLSC_eglChooseConfig,
  GLSC_eglCopyBuffers,
  GLSC_eglCreateContext,
  GLSC_eglCreatePbufferSurface,
  GLSC_eglCreatePixmapSurface,
  GLSC_eglCreateWindowSurface,
  GLSC_eglDestroyContext,
  GLSC_eglDestroySurface,
  GLSC_eglGetConfigAttrib,
  GLSC_eglGetConfigs,
  GLSC_eglGetCurrentContext,
  GLSC_eglGetCurrentDisplay,
  GLSC_eglGetCurrentSurface,
  GLSC_eglGetDisplay,
  GLSC_eglGetError,
// This will never get streamed. Proc address is different.
  // GLSC_eglGetProcAddress,
  GLSC_eglInitialize,
  GLSC_eglMakeCurrent,
  GLSC_eglQueryContext,
  GLSC_eglQueryString,
  GLSC_eglQuerySurface,
  GLSC_eglReleaseTexImage,
  GLSC_eglSurfaceAttrib,
  GLSC_eglSwapBuffers,
  GLSC_eglTerminate,
  GLSC_eglWaitGL,
  GLSC_eglWaitNative,
  
  // GLES 2 commands
  GLSC_glActiveTexture,
  GLSC_glAttachShader,
  GLSC_glBindAttribLocation,
  GLSC_glBindBuffer,
  GLSC_glBindFramebuffer,
  GLSC_glBindTexture,
  GLSC_glBlendEquationSeparate,
  GLSC_glBlendFuncSeparate,
  GLSC_glBlendFunc,
  GLSC_glBufferData,
  GLSC_glBufferSubData,
  GLSC_glClear,
  GLSC_glClearColor,
  GLSC_glClearDepthf,
  GLSC_glClearStencil,
  GLSC_glColorMask,
  GLSC_glCompileShader,
  GLSC_glCopyTexSubImage2D,
  GLSC_glCreateProgram,
  GLSC_glCreateShader,
  GLSC_glCullFace,
  GLSC_glDeleteBuffers,
  GLSC_glDeleteProgram,
  GLSC_glDeleteShader,
  GLSC_glDeleteTextures,
  GLSC_glDepthFunc,
  GLSC_glDepthMask,
  GLSC_glDepthRangef,
  GLSC_glDisable,
  GLSC_glDisableVertexAttribArray,
  GLSC_glDrawArrays,
  GLSC_glDrawElements,
  GLSC_glEnable,
  GLSC_glEnableVertexAttribArray,
  GLSC_glFinish,
  GLSC_glFlush,
  GLSC_glGenBuffers,
  GLSC_glGenTextures,
  GLSC_glGetActiveAttrib,
  GLSC_glGetActiveUniform,
  GLSC_glGetAttribLocation,
  GLSC_glGetError,
  GLSC_glGetFloatv,
  GLSC_glGetProgramiv,
  GLSC_glGetIntegerv,
  GLSC_glGetProgramInfoLog,
  GLSC_glGetShaderInfoLog,
  GLSC_glGetShaderiv,
  GLSC_glGetString,
  GLSC_glGetUniformLocation,
  GLSC_glHint,
  GLSC_glLineWidth,
  GLSC_glLinkProgram,

  GLSC_glPixelStorei,
  GLSC_glPolygonOffset,
  GLSC_glReadPixels,
  GLSC_glShaderSource,
  GLSC_glStencilFunc,
  GLSC_glStencilMask,
  GLSC_glStencilOp,
  GLSC_glTexImage2D,
  GLSC_glTexParameteri,
  GLSC_glTexSubImage2D,
  GLSC_glUniform1f,
  GLSC_glUniform4fv,
  GLSC_glUniformMatrix4fv,
  GLSC_glUseProgram,
  GLSC_glVertexAttribFloat,
  GLSC_glVertexAttribPointer,
  GLSC_glViewport,
  
// OES / EXT extension commands
  GLSC_glMapBufferOES,
  GLSC_glUnmapBufferOES,

  // GLES 3.0 commands
  GLSC_glReadBuffer,
  GLSC_glDrawRangeElements,
  GLSC_glTexImage3D,
  GLSC_glTexSubImage3D,
  GLSC_glCopyTexSubImage3D,
  GLSC_glCompressedTexImage3D,
  GLSC_glCompressedTexSubImage3D,
  GLSC_glGenQueries,
  GLSC_glDeleteQueries,
  GLSC_glIsQuery,
  GLSC_glBeginQuery,
  GLSC_glEndQuery,
  GLSC_glGetQueryiv,
  GLSC_glGetQueryObjectuiv,
  GLSC_glUnmapBuffer,
  GLSC_glGetBufferPointerv,
  GLSC_glDrawBuffers,
  GLSC_glUniformMatrix2x3fv,
  GLSC_glUniformMatrix3x2fv,
  GLSC_glUniformMatrix2x4fv,
  GLSC_glUniformMatrix4x2fv,
  GLSC_glUniformMatrix3x4fv,
  GLSC_glUniformMatrix4x3fv,
  GLSC_glBlitFramebuffer,
  GLSC_glRenderbufferStorageMultisample,
  GLSC_glFramebufferTextureLayer,
  GLSC_glMapBufferRange,
  GLSC_glFlushMappedBufferRange,
  GLSC_glBindVertexArray,
  GLSC_glDeleteVertexArrays,
  GLSC_glGenVertexArrays,
  GLSC_glIsVertexArray,
  GLSC_glGetIntegeri_v,
  GLSC_glBeginTransformFeedback,
  GLSC_glEndTransformFeedback,
  GLSC_glBindBufferRange,
  GLSC_glBindBufferBase,
  GLSC_glTransformFeedbackVaryings,
  GLSC_glGetTransformFeedbackVarying,
  GLSC_glVertexAttribIPointer,
  GLSC_glGetVertexAttribIiv,
  GLSC_glGetVertexAttribIuiv,
  GLSC_glVertexAttribI4i,
  GLSC_glVertexAttribI4ui,
  GLSC_glVertexAttribI4iv,
  GLSC_glVertexAttribI4uiv,
  GLSC_glGetUniformuiv,
  GLSC_glGetFragDataLocation,
  GLSC_glUniform1ui,
  GLSC_glUniform2ui,
  GLSC_glUniform3ui,
  GLSC_glUniform4ui,
  GLSC_glUniform1uiv,
  GLSC_glUniform2uiv,
  GLSC_glUniform3uiv,
  GLSC_glUniform4uiv,
  GLSC_glClearBufferiv,
  GLSC_glClearBufferuiv,
  GLSC_glClearBufferfv,
  GLSC_glClearBufferfi,
  GLSC_glGetStringi,
  GLSC_glCopyBufferSubData,
  GLSC_glGetUniformIndices,
  GLSC_glGetActiveUniformsiv,
  GLSC_glGetUniformBlockIndex,
  GLSC_glGetActiveUniformBlockiv,
  GLSC_glGetActiveUniformBlockName,
  GLSC_glUniformBlockBinding,
  GLSC_glDrawArraysInstanced,
  GLSC_glDrawElementsInstanced,
  GLSC_glFenceSync,
  GLSC_glIsSync,
  GLSC_glDeleteSync,
  GLSC_glClientWaitSync,
  GLSC_glWaitSync,
  GLSC_glGetInteger64v,
  GLSC_glGetSynciv,
  GLSC_glGetInteger64i_v,
  GLSC_glGetBufferParameteri64v,
  GLSC_glGenSamplers,
  GLSC_glDeleteSamplers,
  GLSC_glIsSampler,
  GLSC_glBindSampler,
  GLSC_glSamplerParameteri,
  GLSC_glSamplerParameteriv,
  GLSC_glSamplerParameterf,
  GLSC_glSamplerParameterfv,
  GLSC_glGetSamplerParameteriv,
  GLSC_glGetSamplerParameterfv,
  GLSC_glVertexAttribDivisor,
  GLSC_glBindTransformFeedback,
  GLSC_glDeleteTransformFeedbacks,
  GLSC_glGenTransformFeedbacks,
  GLSC_glIsTransformFeedback,
  GLSC_glPauseTransformFeedback,
  GLSC_glResumeTransformFeedback,
  GLSC_glGetProgramBinary,
  GLSC_glProgramBinary,
  GLSC_glProgramParameteri,
  GLSC_glInvalidateFramebuffer,
  GLSC_glInvalidateSubFramebuffer,
  GLSC_glTexStorage2D,
  GLSC_glTexStorage3D,
  GLSC_glGetInternalformativ,

  // GLES 3.1 commands
  GLSC_glDispatchCompute,
  GLSC_glDispatchComputeIndirect,
  GLSC_glDrawArraysIndirect,
  GLSC_glDrawElementsIndirect,
  GLSC_glFramebufferParameteri,
  GLSC_glGetFramebufferParameteriv,
  GLSC_glGetProgramInterfaceiv,
  GLSC_glGetProgramResourceIndex,
  GLSC_glGetProgramResourceName,
  GLSC_glGetProgramResourceiv,
  GLSC_glGetProgramResourceLocation,
  GLSC_glUseProgramStages,
  GLSC_glActiveShaderProgram,
  GLSC_glCreateShaderProgramv,
  GLSC_glBindProgramPipeline,
  GLSC_glDeleteProgramPipelines,
  GLSC_glGenProgramPipelines,
  GLSC_glIsProgramPipeline,
  GLSC_glGetProgramPipelineiv,
  GLSC_glProgramUniform1i,
  GLSC_glProgramUniform2i,
  GLSC_glProgramUniform3i,
  GLSC_glProgramUniform4i,
  GLSC_glProgramUniform1ui,
  GLSC_glProgramUniform2ui,
  GLSC_glProgramUniform3ui,
  GLSC_glProgramUniform4ui,
  GLSC_glProgramUniform1f,
  GLSC_glProgramUniform2f,
  GLSC_glProgramUniform3f,
  GLSC_glProgramUniform4f,
  GLSC_glProgramUniform1iv,
  GLSC_glProgramUniform2iv,
  GLSC_glProgramUniform3iv,
  GLSC_glProgramUniform4iv,
  GLSC_glProgramUniform1uiv,
  GLSC_glProgramUniform2uiv,
  GLSC_glProgramUniform3uiv,
  GLSC_glProgramUniform4uiv,
  GLSC_glProgramUniform1fv,
  GLSC_glProgramUniform2fv,
  GLSC_glProgramUniform3fv,
  GLSC_glProgramUniform4fv,
  GLSC_glProgramUniformMatrix2fv,
  GLSC_glProgramUniformMatrix3fv,
  GLSC_glProgramUniformMatrix4fv,
  GLSC_glProgramUniformMatrix2x3fv,
  GLSC_glProgramUniformMatrix3x2fv,
  GLSC_glProgramUniformMatrix2x4fv,
  GLSC_glProgramUniformMatrix4x2fv,
  GLSC_glProgramUniformMatrix3x4fv,
  GLSC_glProgramUniformMatrix4x3fv,
  GLSC_glValidateProgramPipeline,
  GLSC_glGetProgramPipelineInfoLog,
  GLSC_glBindImageTexture,
  GLSC_glGetBooleani_v,
  GLSC_glMemoryBarrier,
  GLSC_glMemoryBarrierByRegion,
  GLSC_glTexStorage2DMultisample,
  GLSC_glGetMultisamplefv,
  GLSC_glSampleMaski,
  GLSC_glGetTexLevelParameteriv,
  GLSC_glGetTexLevelParameterfv,
  GLSC_glBindVertexBuffer,
  GLSC_glVertexAttribFormat,
  GLSC_glVertexAttribIFormat,
  GLSC_glVertexAttribBinding,
  GLSC_glVertexBindingDivisor,

  // GLES 3.2 commands
  GLSC_glBlendBarrier,
  GLSC_glCopyImageSubData,
  GLSC_glDebugMessageControl,
  GLSC_glDebugMessageInsert,
  GLSC_glDebugMessageCallback,
  GLSC_glGetDebugMessageLog,
  GLSC_glPushDebugGroup,
  GLSC_glPopDebugGroup,
  GLSC_glObjectLabel,
  GLSC_glGetObjectLabel,
  GLSC_glObjectPtrLabel,
  GLSC_glGetObjectPtrLabel,
  GLSC_glGetPointerv,
  GLSC_glEnablei,
  GLSC_glDisablei,
  GLSC_glBlendEquationi,
  GLSC_glBlendEquationSeparatei,
  GLSC_glBlendFunci,
  GLSC_glBlendFuncSeparatei,
  GLSC_glColorMaski,
  GLSC_glIsEnabledi,
  GLSC_glDrawElementsBaseVertex,
  GLSC_glDrawRangeElementsBaseVertex,
  GLSC_glDrawElementsInstancedBaseVertex,
  GLSC_glFramebufferTexture,
  GLSC_glPrimitiveBoundingBox,
  GLSC_glGetGraphicsResetStatus,
  GLSC_glReadnPixels,
  GLSC_glGetnUniformfv,
  GLSC_glGetnUniformiv,
  GLSC_glGetnUniformuiv,
  GLSC_glMinSampleShading,
  GLSC_glPatchParameteri,
  GLSC_glTexParameterIiv,
  GLSC_glTexParameterIuiv,
  GLSC_glGetTexParameterIiv,
  GLSC_glGetTexParameterIuiv,
  GLSC_glSamplerParameterIiv,
  GLSC_glSamplerParameterIuiv,
  GLSC_glGetSamplerParameterIiv,
  GLSC_glGetSamplerParameterIuiv,
  GLSC_glTexBuffer,
  GLSC_glTexBufferRange,
  GLSC_glTexStorage3DMultisample
};


typedef struct
{
  char *buf;
  size_t size;
  uint32_t ptr;
} gls_buffer_t;


typedef struct
{
  uint32_t cmd;
} gls_command_t;


typedef struct
{
  uint32_t cmd;
  uint32_t frame;
} gls_cmd_flip_t;


typedef struct
{
  uint32_t cmd;
  uint32_t frame;
} gls_ret_flip_t;


typedef struct
{
  uint32_t cmd;
} gls_cmd_get_context_t;


typedef struct
{
  uint32_t cmd;
  uint32_t server_version;
  uint32_t screen_width;
  uint32_t screen_height;
} gls_ret_get_context_t;


typedef union
{
  float data_float[GLS_DATA_SIZE];
  uint32_t data_uint[GLS_DATA_SIZE];
  int32_t data_int[GLS_DATA_SIZE];
  char data_char[GLS_DATA_SIZE * 36]; // * 4
} gls_data_t;


typedef struct
{
  uint32_t cmd;
  uint32_t offset;
  uint32_t size;
  int32_t isLast;
  gls_data_t data;
} gls_cmd_send_data_t;

/* Conversation
 *
 * - boolean, enum, unsigned int
 * '-> uint32_t
 */

// EGL command data
typedef struct
{
  uint32_t attrib_list[GLS_DATA_SIZE];
} gls_data_egl_attriblist_t;
 
// EGL commands
typedef struct
{
  uint32_t cmd;
  uint32_t api;
} gls_eglBindAPI_t;


typedef struct
{
  uint32_t cmd;
  uint32_t success;
} gls_ret_eglBindAPI_t;


typedef struct
{
  uint32_t cmd;
  uint32_t dpy;
  uint32_t surface;
  uint32_t buffer;
} gls_eglBindTexImage_t;


typedef struct
{
  uint32_t cmd;
  uint32_t success;
} gls_ret_eglBindTexImage_t;


typedef struct
{
  uint32_t cmd;
  uint32_t dpy;
  uint32_t config_size;
} gls_eglChooseConfig_t;


typedef struct
{
  uint32_t cmd;
// FIXME sending return data???
  char configs[GLS_DATA_SIZE];
  uint32_t num_config;
  uint32_t success;
} gls_ret_eglChooseConfig_t;


typedef struct
{
  uint32_t cmd;
  uint32_t dpy;
  uint32_t config;
  uint32_t attribute;
} gls_eglGetConfigAttrib_t;


typedef struct
{
  uint32_t cmd;
  uint32_t value;
  uint32_t success;
} gls_ret_eglGetConfigAttrib_t;


typedef struct
{
  uint32_t cmd;
  uint32_t dpy;
  uint32_t config_size;
} gls_eglGetConfigs_t;


typedef struct
{
  uint32_t cmd;
// FIXME sending return data???
  uint32_t configs[GLS_DATA_SIZE];
  uint32_t num_config;
  uint32_t success;
} gls_ret_eglGetConfigs_t;


typedef struct
{
  uint32_t cmd;
} gls_eglGetCurrentContext_t;


typedef struct
{
  uint32_t cmd;
  uint32_t context;
} gls_ret_eglGetCurrentContext_t;


typedef struct
{
  uint32_t cmd;
} gls_eglGetCurrentDisplay_t;


typedef struct
{
  uint32_t cmd;
  uint32_t display;
} gls_ret_eglGetCurrentDisplay_t;


typedef struct
{
  uint32_t cmd;
  uint32_t readdraw;
} gls_eglGetCurrentSurface_t;


typedef struct
{
  uint32_t cmd;
  uint32_t surface;
} gls_ret_eglGetCurrentSurface_t;


typedef struct
{
  uint32_t cmd;
  uint32_t display;
} gls_eglGetDisplay_t;


typedef struct
{
  uint32_t cmd;
  uint32_t display;
} gls_ret_eglGetDisplay_t;


typedef struct
{
  uint32_t cmd;
} gls_eglGetError_t;


typedef struct
{
  uint32_t cmd;
  uint32_t error;
} gls_ret_eglGetError_t;


typedef struct
{
  uint32_t cmd;
  uint32_t dpy;
  uint32_t major;
  uint32_t minor;
} gls_eglInitialize_t;


typedef struct
{
  uint32_t cmd;
  uint32_t success;
} gls_ret_eglInitialize_t;

typedef struct
{
  uint32_t cmd;
  uint32_t dpy;
  uint32_t ctx;
  uint32_t attribute;
} gls_eglQueryContext_t;


typedef struct
{
  uint32_t cmd;
  uint32_t value;
  uint32_t success;
} gls_ret_eglQueryContext_t;


typedef struct
{
  uint32_t cmd;
  uint32_t dpy;
  uint32_t name;
} gls_eglQueryString_t;


typedef struct
{
  uint32_t cmd;
  char params[GLS_STRING_SIZE_PLUS];
} gls_ret_eglQueryString_t;


typedef struct
{
  uint32_t cmd;
  uint32_t dpy;
  uint32_t surface;
  uint32_t attribute;
} gls_eglQuerySurface_t;


typedef struct
{
  uint32_t cmd;
  uint32_t value;
  uint32_t success;
} gls_ret_eglQuerySurface_t;


typedef struct
{
  uint32_t cmd;
  uint32_t dpy;
  uint32_t surface;
  uint32_t buffer;
} gls_eglReleaseTexImage_t;


typedef struct
{
  uint32_t cmd;
  uint32_t success;
} gls_ret_eglReleaseTexImage_t;


typedef struct
{
  uint32_t cmd;
  uint32_t dpy;
  uint32_t surface;
  uint32_t attribute;
  uint32_t value;
} gls_eglSurfaceAttrib_t;


typedef struct
{
  uint32_t cmd;
  uint32_t success;
} gls_ret_eglSurfaceAttrib_t;


typedef struct
{
  uint32_t cmd;
  uint32_t dpy;
} gls_eglTerminate_t;


typedef struct
{
  uint32_t cmd;
  uint32_t success;
} gls_ret_eglTerminate_t;

// OpenGL ES commands
typedef struct
{
  uint32_t cmd;
  uint32_t texture;
} gls_glActiveTexture_t;


typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t shader;
} gls_glAttachShader_t;


typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t index;
  char name[GLS_STRING_SIZE_PLUS];
} gls_glBindAttribLocation_t;


typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t buffer;
} gls_glBindBuffer_t;


typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t texture;
} gls_glBindTexture_t;


typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t framebuffer;
} gls_glBindFramebuffer_t;


typedef struct
{
  uint32_t cmd;
  uint32_t srcRGB;
  uint32_t dstRGB;
  uint32_t srcAlpha;
  uint32_t dstAlpha;
} gls_glBlendFuncSeparate_t;


typedef struct
{
  uint32_t cmd;
  uint32_t modeRGB;
  uint32_t modeAlpha;
} gls_glBlendEquationSeparate_t;


typedef struct
{
  uint32_t cmd;
  uint32_t sfactor;
  uint32_t dfactor;
} gls_glBlendFunc_t;


typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t size;
  uint32_t usage;
} gls_glBufferData_t;


typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t offset;
  int32_t size;
} gls_glBufferSubData_t;


typedef struct
{
  uint32_t cmd;
  uint32_t mask;
} gls_glClear_t;


typedef struct
{
  uint32_t cmd;
  float red;
  float green;
  float  blue;
  float alpha;
} gls_glClearColor_t;


typedef struct
{
  uint32_t cmd;
  float depth;
} gls_glClearDepthf_t;


typedef struct
{
  uint32_t cmd;
  uint32_t s;
} gls_glClearStencil_t;


typedef struct
{
  uint32_t cmd;
  uint32_t red;
  uint32_t green;
  uint32_t blue;
  uint32_t alpha;
} gls_glColorMask_t;


typedef struct
{
  uint32_t cmd;
  uint32_t shader;
} gls_glCompileShader_t;


typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t level;
  int32_t xoffset;
  int32_t yoffset;
  int32_t x;
  int32_t y;
  int32_t width;
  int32_t height;
} gls_glCopyTexSubImage2D_t;


typedef struct
{
  uint32_t cmd;
} gls_glCreateProgram_t;


typedef struct
{
  uint32_t cmd;
  uint32_t program;
} gls_ret_glCreateProgram_t;


typedef struct
{
  uint32_t cmd;
  uint32_t type;
} gls_glCreateShader_t;


typedef struct
{
  uint32_t cmd;
  uint32_t obj;
} gls_ret_glCreateShader_t;


typedef struct
{
  uint32_t cmd;
  uint32_t mode;
} gls_glCullFace_t;


typedef struct
{
  uint32_t cmd;
  int32_t n;
} gls_glDeleteBuffers_t;


typedef struct
{
  uint32_t cmd;
  uint32_t program;
} gls_glDeleteProgram_t;


typedef struct
{
  uint32_t cmd;
  uint32_t shader;
} gls_glDeleteShader_t;


typedef struct
{
  uint32_t cmd;
  uint32_t cmd_size;
  int32_t n;
  uint32_t textures[1];
} gls_glDeleteTextures_t;


typedef struct
{
  uint32_t cmd;
  uint32_t func;
} gls_glDepthFunc_t;


typedef struct
{
  uint32_t cmd;
  uint32_t flag;
} gls_glDepthMask_t;


typedef struct
{
  uint32_t cmd;
  uint32_t zNear;
  uint32_t zFar;
} gls_glDepthRangef_t;


typedef struct
{
  uint32_t cmd;
  uint32_t cap;
} gls_glDisable_t;


typedef struct
{
  uint32_t cmd;
  uint32_t index;
} gls_glDisableVertexAttribArray_t;


typedef struct
{
  uint32_t cmd;
  uint32_t mode;
  int32_t first;
  int32_t count;
} gls_glDrawArrays_t;


typedef struct
{
  uint32_t cmd;
  uint32_t cmd_size;
  uint32_t mode;
  int32_t count;
  uint32_t type;
  // uint32_t indices;
  char indices[GLS_STRING_SIZE_PLUS];
  uint32_t indices_isnull;
  uint32_t indices_uint;
} gls_glDrawElements_t;


typedef struct
{
  uint32_t cmd;
  uint32_t cap;
} gls_glEnable_t;


typedef struct
{
  uint32_t cmd;
  uint32_t index;
} gls_glEnableVertexAttribArray_t;


typedef struct
{
  uint32_t cmd;
} gls_glFinish_t;


typedef struct
{
  uint32_t cmd;
} gls_glFlush_t;


typedef struct
{
  uint32_t cmd;
  int32_t n;
  uint32_t buffers;
} gls_glGenBuffers_t;


typedef struct
{
  uint32_t cmd;
  int32_t n;
} gls_glGenTextures_t;


typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t index;
  int32_t bufsize;
} gls_glGetActiveAttrib_t;


typedef struct
{
  uint32_t cmd;
  int32_t length;
  int32_t size;
  uint32_t type;
  char name[GLS_STRING_SIZE_PLUS];
} gls_ret_glGetActiveAttrib_t;



typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t index;
  int32_t bufsize;
} gls_glGetActiveUniform_t;


typedef struct
{
  uint32_t cmd;
  int32_t length;
  int32_t size;
  uint32_t type;
  char name[GLS_STRING_SIZE_PLUS];
} gls_ret_glGetActiveUniform_t;


typedef struct
{
  uint32_t cmd;
  uint32_t program;
  char name[50];
} gls_glGetAttribLocation_t;


typedef struct
{
  uint32_t cmd;
  int32_t index;
} gls_ret_glGetAttribLocation_t;


typedef struct
{
  uint32_t cmd;
} gls_glGetError_t;


typedef struct
{
  uint32_t cmd;
  uint32_t error;
} gls_ret_glGetError_t;


typedef struct
{
  uint32_t cmd;
  uint32_t name;
  int32_t params;
} gls_glGetIntegerv_t;


typedef struct
{
  uint32_t cmd;
  int32_t params;
} gls_ret_glGetIntegerv_t;


typedef struct
{
  uint32_t cmd;
  uint32_t name;
  float params;
} gls_glGetFloatv_t;


typedef struct
{
  uint32_t cmd;
  float params;
} gls_ret_glGetFloatv_t;


typedef struct
{
  uint32_t cmd;
  uint32_t name;
} gls_glGetString_t;


typedef struct
{
  uint32_t cmd;
  char params[GLS_STRING_SIZE_PLUS];
} gls_ret_glGetString_t;


typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t bufsize;
} gls_glGetProgramInfoLog_t;


typedef struct
{
  uint32_t cmd;
  int32_t length;
  char infolog[GLS_STRING_SIZE_PLUS];
} gls_ret_glGetProgramInfoLog_t;


typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t pname;
} gls_glGetProgramiv_t;


typedef struct
{
  uint32_t cmd;
  int32_t params;
} gls_ret_glGetProgramiv_t;


typedef struct
{
  uint32_t cmd;
  uint32_t shader;
  uint32_t pname;
} gls_glGetShaderiv_t;


typedef struct
{
  uint32_t cmd;
  int32_t params;
} gls_ret_glGetShaderiv_t;


typedef struct
{
  uint32_t cmd;
  uint32_t shader;
  int32_t bufsize;
} gls_glGetShaderInfoLog_t;


typedef struct
{
  uint32_t cmd;
  int32_t length;
  char infolog[GLS_STRING_SIZE_PLUS];
} gls_ret_glGetShaderInfoLog_t;


typedef struct
{
  uint32_t cmd;
  uint32_t program;
  char name[100];
} gls_glGetUniformLocation_t;


typedef struct
{
  uint32_t cmd;
  int32_t location;
} gls_ret_glGetUniformLocation_t;


typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t mode;
} gls_glHint_t;


typedef struct
{
  uint32_t cmd;
  float width;
} gls_glLineWidth_t;


typedef struct
{
  uint32_t cmd;
  uint32_t program;
} gls_glLinkProgram_t;


typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t access;
} gls_glMapBufferOES_t;


typedef struct
{
  uint32_t cmd;
  uint32_t pname;
  int32_t param;
} gls_glPixelStorei_t;


typedef struct
{
  uint32_t cmd;
  float factor;
  float units;
} gls_glPolygonOffset_t;


typedef struct
{
  uint32_t cmd;
  int32_t x;
  int32_t y;
  int32_t width;
  int32_t height;
  uint32_t format;
  uint32_t type;
} gls_glReadPixels_t;


typedef struct
{
  uint32_t cmd;
  char pixels[4];
} gls_ret_glReadPixels_t;


typedef struct
{
  uint32_t cmd;
  uint32_t shader;
  int32_t count;
} gls_glShaderSource_t;


typedef struct
{
  uint32_t string[GLS_STRING_SIZE_PLUS];
  int32_t length[GLS_STRING_SIZE_PLUS];
  char data[4];
} gls_data_glShaderSource_t;


typedef struct
{
  uint32_t cmd;
  uint32_t func;
  int32_t r;
  uint32_t m;
} gls_glStencilFunc_t;


typedef struct
{
  uint32_t cmd;
  uint32_t fail;
  uint32_t zfail;
  uint32_t zpass;
} gls_glStencilOp_t;


typedef struct
{
  uint32_t cmd;
  uint32_t mask;
} gls_glStencilMask_t;


typedef struct
{
  uint32_t cmd;
  uint32_t cmd_size;
  uint32_t target;
  int32_t level;
  int32_t internalformat;
  int32_t width;
  int32_t height;
  int32_t border;
  uint32_t format;
  uint32_t type;
  char pixels[4];
  uint32_t pixels_isnull;
} gls_glTexImage2D_t;


typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t pname;
  int32_t param;
} gls_glTexParameteri_t;


typedef struct
{
  uint32_t cmd;
  uint32_t cmd_size;
  uint32_t target;
  int32_t level;
  int32_t xoffset;
  int32_t yoffset;
  int32_t width;
  int32_t height;
  uint32_t format;
  uint32_t type;
  char pixels[4];
  uint32_t pixels_isnull;
} gls_glTexSubImage2D_t;


typedef struct
{
  uint32_t cmd;
  int32_t location;
  float x;
} gls_glUniform1f_t;


typedef struct
{
  uint32_t cmd;
  uint32_t cmd_size;
  int32_t location;
  int32_t count;
  float v[1];
} gls_glUniform4fv_t;


typedef struct
{
  uint32_t cmd;
  uint32_t cmd_size;
  int32_t location;
  int32_t count;
  uint32_t transpose;
  float value[1];
} gls_glUniformMatrix4fv_t;


typedef struct
{
  uint32_t cmd;
  uint32_t target;
} gls_glUnmapBufferOES_t;


typedef struct
{
  uint32_t cmd;
  uint32_t success;
} gls_ret_glUnmapBufferOES_t;


typedef struct
{
  uint32_t cmd;
  uint32_t program;
} gls_glUseProgram_t;


// glVertexAttrib*f and glVertexAttrib*fv combine
typedef struct
{
  uint32_t cmd;
  uint32_t index;
  uint32_t num_float;
  uint32_t call_arr;
} gls_glVertexAttribFloat_t;
typedef struct
{
  float arr[4];
} gls_data_glVertexAttribFloat_t;


typedef struct
{
  uint32_t cmd;
  uint32_t indx;
  int32_t size;
  uint32_t type;
  int32_t stride;
  // Should it be huge?
  char ptr[GLS_STRING_SIZE];
  uint32_t ptr_uint;
  uint32_t ptr_isnull;
  uint32_t normalized;
} gls_glVertexAttribPointer_t;


typedef struct
{
  uint32_t cmd;
  int32_t x;
  int32_t y;
  int32_t width;
  int32_t height;
} gls_glViewport_t;


/*
 * OES / EXT extension commands
 */
typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t internalformat;
  int32_t width;
  uint32_t format;
  uint32_t type;
  char image[GLS_STRING_SIZE_PLUS];
} gls_glConvolutionFilter1D_t;


// GLES 3.0 command data
typedef struct
{
  uint32_t cmd;
  uint32_t src;
} gls_glReadBuffer_t;

typedef struct
{
  uint32_t cmd;
  uint32_t cmd_size;
  uint32_t mode;
  uint32_t start;
  uint32_t end;
  int32_t count;
  uint32_t type;
  char indices[GLS_STRING_SIZE_PLUS];
  uint32_t indices_isnull;
  uint32_t indices_uint;
} gls_glDrawRangeElements_t;

typedef struct
{
  uint32_t cmd;
  uint32_t cmd_size;
  uint32_t target;
  int32_t level;
  int32_t internalformat;
  int32_t width;
  int32_t height;
  int32_t depth;
  int32_t border;
  uint32_t format;
  uint32_t type;
  char pixels[4];
  uint32_t pixels_isnull;
} gls_glTexImage3D_t;

typedef struct
{
  uint32_t cmd;
  uint32_t cmd_size;
  uint32_t target;
  int32_t level;
  int32_t xoffset;
  int32_t yoffset;
  int32_t zoffset;
  int32_t width;
  int32_t height;
  int32_t depth;
  uint32_t format;
  uint32_t type;
  char pixels[4];
  uint32_t pixels_isnull;
} gls_glTexSubImage3D_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t level;
  int32_t xoffset;
  int32_t yoffset;
  int32_t zoffset;
  int32_t x;
  int32_t y;
  int32_t width;
  int32_t height;
} gls_glCopyTexSubImage3D_t;

typedef struct
{
  uint32_t cmd;
  uint32_t cmd_size;
  uint32_t target;
  int32_t level;
  uint32_t internalformat;
  int32_t width;
  int32_t height;
  int32_t depth;
  int32_t border;
  int32_t imageSize;
  char data[4];
} gls_glCompressedTexImage3D_t;

typedef struct
{
  uint32_t cmd;
  uint32_t cmd_size;
  uint32_t target;
  int32_t level;
  int32_t xoffset;
  int32_t yoffset;
  int32_t zoffset;
  int32_t width;
  int32_t height;
  int32_t depth;
  uint32_t format;
  int32_t imageSize;
  char data[4];
} gls_glCompressedTexSubImage3D_t;

typedef struct
{
  uint32_t cmd;
  int32_t n;
} gls_glGenQueries_t;

typedef struct
{
  uint32_t cmd;
  int32_t n;
} gls_glDeleteQueries_t;

typedef struct
{
  uint32_t cmd;
  uint32_t id;
} gls_glIsQuery_t;

typedef struct
{
  uint32_t cmd;
  uint32_t result;
} gls_ret_glIsQuery_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t id;
} gls_glBeginQuery_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
} gls_glEndQuery_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t pname;
} gls_glGetQueryiv_t;

typedef struct
{
  uint32_t cmd;
  int32_t params[4];
} gls_ret_glGetQueryiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t id;
  uint32_t pname;
} gls_glGetQueryObjectuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t params[4];
} gls_ret_glGetQueryObjectuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
} gls_glUnmapBuffer_t;

typedef struct
{
  uint32_t cmd;
  uint32_t success;
} gls_ret_glUnmapBuffer_t;

typedef struct
{
  uint32_t cmd;
  int32_t n;
} gls_glDrawBuffers_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glUniformMatrix2x3fv_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glUniformMatrix3x2fv_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glUniformMatrix2x4fv_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glUniformMatrix4x2fv_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glUniformMatrix3x4fv_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glUniformMatrix4x3fv_t;

typedef struct
{
  uint32_t cmd;
  int32_t srcX0;
  int32_t srcY0;
  int32_t srcX1;
  int32_t srcY1;
  int32_t dstX0;
  int32_t dstY0;
  int32_t dstX1;
  int32_t dstY1;
  uint32_t mask;
  uint32_t filter;
} gls_glBlitFramebuffer_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t samples;
  uint32_t internalformat;
  int32_t width;
  int32_t height;
} gls_glRenderbufferStorageMultisample_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t attachment;
  uint32_t texture;
  int32_t level;
  int32_t layer;
} gls_glFramebufferTextureLayer_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t offset;
  int32_t length;
  uint32_t access;
} gls_glMapBufferRange_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t offset;
  int32_t length;
} gls_glFlushMappedBufferRange_t;

typedef struct
{
  uint32_t cmd;
  uint32_t array;
} gls_glBindVertexArray_t;

typedef struct
{
  uint32_t cmd;
  int32_t n;
} gls_glDeleteVertexArrays_t;

typedef struct
{
  uint32_t cmd;
  int32_t n;
} gls_glGenVertexArrays_t;

typedef struct
{
  uint32_t cmd;
  uint32_t array;
} gls_glIsVertexArray_t;

typedef struct
{
  uint32_t cmd;
  uint32_t result;
} gls_ret_glIsVertexArray_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t index;
} gls_glGetIntegeri_v_t;

typedef struct
{
  uint32_t cmd;
  int32_t data[4];
} gls_ret_glGetIntegeri_v_t;

typedef struct
{
  uint32_t cmd;
  uint32_t primitiveMode;
} gls_glBeginTransformFeedback_t;

typedef struct
{
  uint32_t cmd;
} gls_glEndTransformFeedback_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t index;
  uint32_t buffer;
  int32_t offset;
  int32_t size;
} gls_glBindBufferRange_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t index;
  uint32_t buffer;
} gls_glBindBufferBase_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t count;
  uint32_t bufferMode;
} gls_glTransformFeedbackVaryings_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t index;
  int32_t bufSize;
} gls_glGetTransformFeedbackVarying_t;

typedef struct
{
  uint32_t cmd;
  int32_t length;
  int32_t size;
  uint32_t type;
  char name[GLS_STRING_SIZE_PLUS];
} gls_ret_glGetTransformFeedbackVarying_t;

typedef struct
{
  uint32_t cmd;
  uint32_t index;
  int32_t size;
  uint32_t type;
  int32_t stride;
  uint32_t pointer_isnull;
  uint32_t pointer_uint;
} gls_glVertexAttribIPointer_t;

typedef struct
{
  uint32_t cmd;
  uint32_t index;
  uint32_t pname;
} gls_glGetVertexAttribIiv_t;

typedef struct
{
  uint32_t cmd;
  int32_t params[4];
} gls_ret_glGetVertexAttribIiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t index;
  uint32_t pname;
} gls_glGetVertexAttribIuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t params[4];
} gls_ret_glGetVertexAttribIuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t index;
  int32_t x;
  int32_t y;
  int32_t z;
  int32_t w;
} gls_glVertexAttribI4i_t;

typedef struct
{
  uint32_t cmd;
  uint32_t index;
  uint32_t x;
  uint32_t y;
  uint32_t z;
  uint32_t w;
} gls_glVertexAttribI4ui_t;

typedef struct
{
  uint32_t cmd;
  uint32_t index;
} gls_glVertexAttribI4iv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t index;
} gls_glVertexAttribI4uiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
} gls_glGetUniformuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t params[4];
} gls_ret_glGetUniformuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  char name[GLS_STRING_SIZE_PLUS];
} gls_glGetFragDataLocation_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
} gls_ret_glGetFragDataLocation_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  uint32_t v0;
} gls_glUniform1ui_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  uint32_t v0;
  uint32_t v1;
} gls_glUniform2ui_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  uint32_t v0;
  uint32_t v1;
  uint32_t v2;
} gls_glUniform3ui_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  uint32_t v0;
  uint32_t v1;
  uint32_t v2;
  uint32_t v3;
} gls_glUniform4ui_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  int32_t count;
} gls_glUniform1uiv_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  int32_t count;
} gls_glUniform2uiv_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  int32_t count;
} gls_glUniform3uiv_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
  int32_t count;
} gls_glUniform4uiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t buffer;
  int32_t drawbuffer;
} gls_glClearBufferiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t buffer;
  int32_t drawbuffer;
} gls_glClearBufferuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t buffer;
  int32_t drawbuffer;
} gls_glClearBufferfv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t buffer;
  int32_t drawbuffer;
  float depth;
  int32_t stencil;
} gls_glClearBufferfi_t;

typedef struct
{
  uint32_t cmd;
  uint32_t name;
  uint32_t index;
} gls_glGetStringi_t;

typedef struct
{
  uint32_t cmd;
  char params[GLS_STRING_SIZE_PLUS];
} gls_ret_glGetStringi_t;

typedef struct
{
  uint32_t cmd;
  uint32_t readTarget;
  uint32_t writeTarget;
  int32_t readOffset;
  int32_t writeOffset;
  int32_t size;
} gls_glCopyBufferSubData_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t uniformCount;
} gls_glGetUniformIndices_t;

typedef struct
{
  uint32_t cmd;
  uint32_t uniformIndices[GLS_DATA_SIZE];
} gls_ret_glGetUniformIndices_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t uniformCount;
  uint32_t pname;
} gls_glGetActiveUniformsiv_t;

typedef struct
{
  uint32_t cmd;
  int32_t params[GLS_DATA_SIZE];
} gls_ret_glGetActiveUniformsiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  char uniformBlockName[GLS_STRING_SIZE_PLUS];
} gls_glGetUniformBlockIndex_t;

typedef struct
{
  uint32_t cmd;
  uint32_t uniformBlockIndex;
} gls_ret_glGetUniformBlockIndex_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t uniformBlockIndex;
  uint32_t pname;
} gls_glGetActiveUniformBlockiv_t;

typedef struct
{
  uint32_t cmd;
  int32_t params[GLS_DATA_SIZE];
} gls_ret_glGetActiveUniformBlockiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t uniformBlockIndex;
  int32_t bufSize;
} gls_glGetActiveUniformBlockName_t;

typedef struct
{
  uint32_t cmd;
  int32_t length;
  char uniformBlockName[GLS_STRING_SIZE_PLUS];
} gls_ret_glGetActiveUniformBlockName_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t uniformBlockIndex;
  uint32_t uniformBlockBinding;
} gls_glUniformBlockBinding_t;

typedef struct
{
  uint32_t cmd;
  uint32_t mode;
  int32_t first;
  int32_t count;
  int32_t instancecount;
} gls_glDrawArraysInstanced_t;

typedef struct
{
  uint32_t cmd;
  uint32_t mode;
  int32_t count;
  uint32_t type;
  uint32_t indices_isnull;
  uint32_t indices_uint;
  int32_t instancecount;
} gls_glDrawElementsInstanced_t;

typedef struct
{
  uint32_t cmd;
  uint32_t condition;
  uint32_t flags;
} gls_glFenceSync_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sync;
} gls_ret_glFenceSync_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sync;
} gls_glIsSync_t;

typedef struct
{
  uint32_t cmd;
  uint32_t result;
} gls_ret_glIsSync_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sync;
} gls_glDeleteSync_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sync;
  uint32_t flags;
  uint64_t timeout;
} gls_glClientWaitSync_t;

typedef struct
{
  uint32_t cmd;
  uint32_t result;
} gls_ret_glClientWaitSync_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sync;
  uint32_t flags;
  uint64_t timeout;
} gls_glWaitSync_t;

typedef struct
{
  uint32_t cmd;
  uint32_t pname;
} gls_glGetInteger64v_t;

typedef struct
{
  uint32_t cmd;
  int64_t data[4];
} gls_ret_glGetInteger64v_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sync;
  uint32_t pname;
  int32_t bufSize;
} gls_glGetSynciv_t;

typedef struct
{
  uint32_t cmd;
  int32_t length;
  int32_t values[4];
} gls_ret_glGetSynciv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t index;
} gls_glGetInteger64i_v_t;

typedef struct
{
  uint32_t cmd;
  int64_t data[4];
} gls_ret_glGetInteger64i_v_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t pname;
} gls_glGetBufferParameteri64v_t;

typedef struct
{
  uint32_t cmd;
  int64_t params[4];
} gls_ret_glGetBufferParameteri64v_t;

typedef struct
{
  uint32_t cmd;
  int32_t count;
} gls_glGenSamplers_t;

typedef struct
{
  uint32_t cmd;
  int32_t count;
} gls_glDeleteSamplers_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sampler;
} gls_glIsSampler_t;

typedef struct
{
  uint32_t cmd;
  uint32_t result;
} gls_ret_glIsSampler_t;

typedef struct
{
  uint32_t cmd;
  uint32_t unit;
  uint32_t sampler;
} gls_glBindSampler_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sampler;
  uint32_t pname;
  int32_t param;
} gls_glSamplerParameteri_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sampler;
  uint32_t pname;
} gls_glSamplerParameteriv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sampler;
  uint32_t pname;
  float param;
} gls_glSamplerParameterf_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sampler;
  uint32_t pname;
} gls_glSamplerParameterfv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sampler;
  uint32_t pname;
} gls_glGetSamplerParameteriv_t;

typedef struct
{
  uint32_t cmd;
  int32_t params[4];
} gls_ret_glGetSamplerParameteriv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sampler;
  uint32_t pname;
} gls_glGetSamplerParameterfv_t;

typedef struct
{
  uint32_t cmd;
  float params[4];
} gls_ret_glGetSamplerParameterfv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t index;
  uint32_t divisor;
} gls_glVertexAttribDivisor_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t id;
} gls_glBindTransformFeedback_t;

typedef struct
{
  uint32_t cmd;
  int32_t n;
} gls_glDeleteTransformFeedbacks_t;

typedef struct
{
  uint32_t cmd;
  int32_t n;
} gls_glGenTransformFeedbacks_t;

typedef struct
{
  uint32_t cmd;
  uint32_t id;
} gls_glIsTransformFeedback_t;

typedef struct
{
  uint32_t cmd;
  uint32_t result;
} gls_ret_glIsTransformFeedback_t;

typedef struct
{
  uint32_t cmd;
} gls_glPauseTransformFeedback_t;

typedef struct
{
  uint32_t cmd;
} gls_glResumeTransformFeedback_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t bufSize;
} gls_glGetProgramBinary_t;

typedef struct
{
  uint32_t cmd;
  int32_t length;
  uint32_t binaryFormat;
} gls_ret_glGetProgramBinary_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t binaryFormat;
  int32_t length;
} gls_glProgramBinary_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t pname;
  int32_t value;
} gls_glProgramParameteri_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t numAttachments;
} gls_glInvalidateFramebuffer_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t numAttachments;
  int32_t x;
  int32_t y;
  int32_t width;
  int32_t height;
} gls_glInvalidateSubFramebuffer_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t levels;
  uint32_t internalformat;
  int32_t width;
  int32_t height;
} gls_glTexStorage2D_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t levels;
  uint32_t internalformat;
  int32_t width;
  int32_t height;
  int32_t depth;
} gls_glTexStorage3D_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t internalformat;
  uint32_t pname;
  int32_t bufSize;
} gls_glGetInternalformativ_t;

typedef struct
{
  uint32_t cmd;
  int32_t params[GLS_DATA_SIZE];
} gls_ret_glGetInternalformativ_t;

// GLES 3.1 command data
typedef struct
{
  uint32_t cmd;
  uint32_t num_groups_x;
  uint32_t num_groups_y;
  uint32_t num_groups_z;
} gls_glDispatchCompute_t;

typedef struct
{
  uint32_t cmd;
  int32_t indirect;
} gls_glDispatchComputeIndirect_t;

typedef struct
{
  uint32_t cmd;
  uint32_t mode;
  uint32_t indirect_isnull;
  uint32_t indirect_uint;
} gls_glDrawArraysIndirect_t;

typedef struct
{
  uint32_t cmd;
  uint32_t mode;
  uint32_t type;
  uint32_t indirect_isnull;
  uint32_t indirect_uint;
} gls_glDrawElementsIndirect_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t pname;
  int32_t param;
} gls_glFramebufferParameteri_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t pname;
} gls_glGetFramebufferParameteriv_t;

typedef struct
{
  uint32_t cmd;
  int32_t params[4];
} gls_ret_glGetFramebufferParameteriv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t programInterface;
  uint32_t pname;
} gls_glGetProgramInterfaceiv_t;

typedef struct
{
  uint32_t cmd;
  int32_t params[4];
} gls_ret_glGetProgramInterfaceiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t programInterface;
  char name[GLS_STRING_SIZE_PLUS];
} gls_glGetProgramResourceIndex_t;

typedef struct
{
  uint32_t cmd;
  uint32_t index;
} gls_ret_glGetProgramResourceIndex_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t programInterface;
  uint32_t index;
  int32_t bufSize;
} gls_glGetProgramResourceName_t;

typedef struct
{
  uint32_t cmd;
  int32_t length;
  char name[GLS_STRING_SIZE_PLUS];
} gls_ret_glGetProgramResourceName_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t programInterface;
  uint32_t index;
  int32_t propCount;
  int32_t bufSize;
} gls_glGetProgramResourceiv_t;

typedef struct
{
  uint32_t cmd;
  int32_t length;
  int32_t params[GLS_DATA_SIZE];
} gls_ret_glGetProgramResourceiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  uint32_t programInterface;
  char name[GLS_STRING_SIZE_PLUS];
} gls_glGetProgramResourceLocation_t;

typedef struct
{
  uint32_t cmd;
  int32_t location;
} gls_ret_glGetProgramResourceLocation_t;

typedef struct
{
  uint32_t cmd;
  uint32_t pipeline;
  uint32_t stages;
  uint32_t program;
} gls_glUseProgramStages_t;

typedef struct
{
  uint32_t cmd;
  uint32_t pipeline;
  uint32_t program;
} gls_glActiveShaderProgram_t;

typedef struct
{
  uint32_t cmd;
  uint32_t type;
  int32_t count;
} gls_glCreateShaderProgramv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
} gls_ret_glCreateShaderProgramv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t pipeline;
} gls_glBindProgramPipeline_t;

typedef struct
{
  uint32_t cmd;
  int32_t n;
} gls_glDeleteProgramPipelines_t;

typedef struct
{
  uint32_t cmd;
  int32_t n;
} gls_glGenProgramPipelines_t;

typedef struct
{
  uint32_t cmd;
  uint32_t pipeline;
} gls_glIsProgramPipeline_t;

typedef struct
{
  uint32_t cmd;
  uint32_t result;
} gls_ret_glIsProgramPipeline_t;

typedef struct
{
  uint32_t cmd;
  uint32_t pipeline;
  uint32_t pname;
} gls_glGetProgramPipelineiv_t;

typedef struct
{
  uint32_t cmd;
  int32_t params[4];
} gls_ret_glGetProgramPipelineiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t v0;
} gls_glProgramUniform1i_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t v0;
  int32_t v1;
} gls_glProgramUniform2i_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t v0;
  int32_t v1;
  int32_t v2;
} gls_glProgramUniform3i_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t v0;
  int32_t v1;
  int32_t v2;
  int32_t v3;
} gls_glProgramUniform4i_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  uint32_t v0;
} gls_glProgramUniform1ui_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  uint32_t v0;
  uint32_t v1;
} gls_glProgramUniform2ui_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  uint32_t v0;
  uint32_t v1;
  uint32_t v2;
} gls_glProgramUniform3ui_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  uint32_t v0;
  uint32_t v1;
  uint32_t v2;
  uint32_t v3;
} gls_glProgramUniform4ui_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  float v0;
} gls_glProgramUniform1f_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  float v0;
  float v1;
} gls_glProgramUniform2f_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  float v0;
  float v1;
  float v2;
} gls_glProgramUniform3f_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  float v0;
  float v1;
  float v2;
  float v3;
} gls_glProgramUniform4f_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
} gls_glProgramUniform1iv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
} gls_glProgramUniform2iv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
} gls_glProgramUniform3iv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
} gls_glProgramUniform4iv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
} gls_glProgramUniform1uiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
} gls_glProgramUniform2uiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
} gls_glProgramUniform3uiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
} gls_glProgramUniform4uiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
} gls_glProgramUniform1fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
} gls_glProgramUniform2fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
} gls_glProgramUniform3fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
} gls_glProgramUniform4fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glProgramUniformMatrix2fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glProgramUniformMatrix3fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glProgramUniformMatrix4fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glProgramUniformMatrix2x3fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glProgramUniformMatrix3x2fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glProgramUniformMatrix2x4fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glProgramUniformMatrix4x2fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glProgramUniformMatrix3x4fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t count;
  uint32_t transpose;
} gls_glProgramUniformMatrix4x3fv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t pipeline;
} gls_glValidateProgramPipeline_t;

typedef struct
{
  uint32_t cmd;
  uint32_t pipeline;
  int32_t bufSize;
} gls_glGetProgramPipelineInfoLog_t;

typedef struct
{
  uint32_t cmd;
  int32_t length;
  char infoLog[GLS_STRING_SIZE_PLUS];
} gls_ret_glGetProgramPipelineInfoLog_t;

typedef struct
{
  uint32_t cmd;
  uint32_t unit;
  uint32_t texture;
  int32_t level;
  uint32_t layered;
  int32_t layer;
  uint32_t access;
  uint32_t format;
} gls_glBindImageTexture_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t index;
} gls_glGetBooleani_v_t;

typedef struct
{
  uint32_t cmd;
  uint32_t data;
} gls_ret_glGetBooleani_v_t;

typedef struct
{
  uint32_t cmd;
  uint32_t barriers;
} gls_glMemoryBarrier_t;

typedef struct
{
  uint32_t cmd;
  uint32_t barriers;
} gls_glMemoryBarrierByRegion_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t samples;
  uint32_t internalformat;
  int32_t width;
  int32_t height;
  uint32_t fixedsamplelocations;
} gls_glTexStorage2DMultisample_t;

typedef struct
{
  uint32_t cmd;
  uint32_t pname;
  uint32_t index;
} gls_glGetMultisamplefv_t;

typedef struct
{
  uint32_t cmd;
  float val[2];
} gls_ret_glGetMultisamplefv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t maskNumber;
  uint32_t mask;
} gls_glSampleMaski_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t level;
  uint32_t pname;
} gls_glGetTexLevelParameteriv_t;

typedef struct
{
  uint32_t cmd;
  int32_t params[4];
} gls_ret_glGetTexLevelParameteriv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t level;
  uint32_t pname;
} gls_glGetTexLevelParameterfv_t;

typedef struct
{
  uint32_t cmd;
  float params[4];
} gls_ret_glGetTexLevelParameterfv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t bindingindex;
  uint32_t buffer;
  int32_t offset;
  int32_t stride;
} gls_glBindVertexBuffer_t;

typedef struct
{
  uint32_t cmd;
  uint32_t attribindex;
  int32_t size;
  uint32_t type;
  uint32_t normalized;
  uint32_t relativeoffset;
} gls_glVertexAttribFormat_t;

typedef struct
{
  uint32_t cmd;
  uint32_t attribindex;
  int32_t size;
  uint32_t type;
  uint32_t relativeoffset;
} gls_glVertexAttribIFormat_t;

typedef struct
{
  uint32_t cmd;
  uint32_t attribindex;
  uint32_t bindingindex;
} gls_glVertexAttribBinding_t;

typedef struct
{
  uint32_t cmd;
  uint32_t bindingindex;
  uint32_t divisor;
} gls_glVertexBindingDivisor_t;

// GLES 3.2 command data
typedef struct
{
  uint32_t cmd;
} gls_glBlendBarrier_t;

typedef struct
{
  uint32_t cmd;
  uint32_t srcName;
  uint32_t srcTarget;
  int32_t srcLevel;
  int32_t srcX;
  int32_t srcY;
  int32_t srcZ;
  uint32_t dstName;
  uint32_t dstTarget;
  int32_t dstLevel;
  int32_t dstX;
  int32_t dstY;
  int32_t dstZ;
  int32_t srcWidth;
  int32_t srcHeight;
  int32_t srcDepth;
} gls_glCopyImageSubData_t;

typedef struct
{
  uint32_t cmd;
  uint32_t source;
  uint32_t type;
  uint32_t severity;
  int32_t count;
  uint32_t enabled;
} gls_glDebugMessageControl_t;

typedef struct
{
  uint32_t cmd;
  uint32_t source;
  uint32_t type;
  uint32_t id;
  uint32_t severity;
  int32_t length;
} gls_glDebugMessageInsert_t;

typedef struct
{
  uint32_t cmd;
} gls_glDebugMessageCallback_t;

typedef struct
{
  uint32_t cmd;
  uint32_t count;
  int32_t bufSize;
} gls_glGetDebugMessageLog_t;

typedef struct
{
  uint32_t cmd;
  uint32_t source;
  uint32_t id;
  int32_t length;
} gls_glPushDebugGroup_t;

typedef struct
{
  uint32_t cmd;
} gls_glPopDebugGroup_t;

typedef struct
{
  uint32_t cmd;
  uint32_t identifier;
  uint32_t name;
  int32_t length;
} gls_glObjectLabel_t;

typedef struct
{
  uint32_t cmd;
  uint32_t identifier;
  uint32_t name;
  int32_t bufSize;
} gls_glGetObjectLabel_t;

typedef struct
{
  uint32_t cmd;
  int32_t length;
  char label[GLS_STRING_SIZE_PLUS];
} gls_ret_glGetObjectLabel_t;

typedef struct
{
  uint32_t cmd;
  int32_t length;
} gls_glObjectPtrLabel_t;

typedef struct
{
  uint32_t cmd;
  int32_t bufSize;
} gls_glGetObjectPtrLabel_t;

typedef struct
{
  uint32_t cmd;
  int32_t length;
  char label[GLS_STRING_SIZE_PLUS];
} gls_ret_glGetObjectPtrLabel_t;

typedef struct
{
  uint32_t cmd;
  uint32_t pname;
} gls_glGetPointerv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t index;
} gls_glEnablei_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t index;
} gls_glDisablei_t;

typedef struct
{
  uint32_t cmd;
  uint32_t buf;
  uint32_t mode;
} gls_glBlendEquationi_t;

typedef struct
{
  uint32_t cmd;
  uint32_t buf;
  uint32_t modeRGB;
  uint32_t modeAlpha;
} gls_glBlendEquationSeparatei_t;

typedef struct
{
  uint32_t cmd;
  uint32_t buf;
  uint32_t src;
  uint32_t dst;
} gls_glBlendFunci_t;

typedef struct
{
  uint32_t cmd;
  uint32_t buf;
  uint32_t srcRGB;
  uint32_t dstRGB;
  uint32_t srcAlpha;
  uint32_t dstAlpha;
} gls_glBlendFuncSeparatei_t;

typedef struct
{
  uint32_t cmd;
  uint32_t index;
  uint32_t r;
  uint32_t g;
  uint32_t b;
  uint32_t a;
} gls_glColorMaski_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t index;
} gls_glIsEnabledi_t;

typedef struct
{
  uint32_t cmd;
  uint32_t result;
} gls_ret_glIsEnabledi_t;

typedef struct
{
  uint32_t cmd;
  uint32_t mode;
  int32_t count;
  uint32_t type;
  int32_t basevertex;
} gls_glDrawElementsBaseVertex_t;

typedef struct
{
  uint32_t cmd;
  uint32_t mode;
  uint32_t start;
  uint32_t end;
  int32_t count;
  uint32_t type;
  int32_t basevertex;
} gls_glDrawRangeElementsBaseVertex_t;

typedef struct
{
  uint32_t cmd;
  uint32_t mode;
  int32_t count;
  uint32_t type;
  int32_t instancecount;
  int32_t basevertex;
} gls_glDrawElementsInstancedBaseVertex_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t attachment;
  uint32_t texture;
  int32_t level;
} gls_glFramebufferTexture_t;

typedef struct
{
  uint32_t cmd;
  float minX;
  float minY;
  float minZ;
  float minW;
  float maxX;
  float maxY;
  float maxZ;
  float maxW;
} gls_glPrimitiveBoundingBox_t;

typedef struct
{
  uint32_t cmd;
} gls_glGetGraphicsResetStatus_t;

typedef struct
{
  uint32_t cmd;
  uint32_t status;
} gls_ret_glGetGraphicsResetStatus_t;

typedef struct
{
  uint32_t cmd;
  int32_t x;
  int32_t y;
  int32_t width;
  int32_t height;
  uint32_t format;
  uint32_t type;
  int32_t bufSize;
} gls_glReadnPixels_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t bufSize;
} gls_glGetnUniformfv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t bufSize;
} gls_glGetnUniformiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t program;
  int32_t location;
  int32_t bufSize;
} gls_glGetnUniformuiv_t;

typedef struct
{
  uint32_t cmd;
  float value;
} gls_glMinSampleShading_t;

typedef struct
{
  uint32_t cmd;
  uint32_t pname;
  int32_t value;
} gls_glPatchParameteri_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t pname;
} gls_glTexParameterIiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t pname;
} gls_glTexParameterIuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t pname;
} gls_glGetTexParameterIiv_t;

typedef struct
{
  uint32_t cmd;
  int32_t params[4];
} gls_ret_glGetTexParameterIiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t pname;
} gls_glGetTexParameterIuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t params[4];
} gls_ret_glGetTexParameterIuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sampler;
  uint32_t pname;
} gls_glSamplerParameterIiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sampler;
  uint32_t pname;
} gls_glSamplerParameterIuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sampler;
  uint32_t pname;
} gls_glGetSamplerParameterIiv_t;

typedef struct
{
  uint32_t cmd;
  int32_t params[4];
} gls_ret_glGetSamplerParameterIiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t sampler;
  uint32_t pname;
} gls_glGetSamplerParameterIuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t params[4];
} gls_ret_glGetSamplerParameterIuiv_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t internalformat;
  uint32_t buffer;
} gls_glTexBuffer_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  uint32_t internalformat;
  uint32_t buffer;
  int32_t offset;
  int32_t size;
} gls_glTexBufferRange_t;

typedef struct
{
  uint32_t cmd;
  uint32_t target;
  int32_t samples;
  uint32_t internalformat;
  int32_t width;
  int32_t height;
  int32_t depth;
  uint32_t fixedsamplelocations;
} gls_glTexStorage3DMultisample_t;


/*
typedef struct
{
  uint32_t cmd;
  uint32_t ;
} gls__t;

typedef void             GLvoid;
typedef char             GLchar;
typedef unsigned int     GLenum;
typedef unsigned char    GLboolean;
typedef unsigned int     GLbitfield;
typedef signed   char    GLbyte;
typedef short            GLshort;
typedef int              GLint;
typedef int              GLsizei;
typedef unsigned char    GLubyte;
typedef unsigned short   GLushort;
typedef unsigned int     GLuint;
typedef float            GLfloat;
typedef float            GLclampf;
typedef int32_t          GLfixed;
typedef signed long int  GLintptr;
typedef signed long int  GLsizeiptr;
 */


static inline uint32_t next_ptr(uint32_t ptr, uint32_t size, uint32_t alignment_bits)
{
  uint32_t a = (1 << alignment_bits) - 1;
  return (ptr + size + a) & ~a;
}


static inline void gls_sleep(unsigned int max_mbps, unsigned int size)
{
  //unsigned int sleeptime = (1.0f / (a->max_mbps * 1024.0f * 1024.0f / 8.0f / (float)(size)) * 1000000.0f) + 1;
  unsigned int sleeptime = (float)1500.0f * 7.62939453125f / max_mbps + 1.0f;
  usleep((useconds_t)sleeptime);
}


