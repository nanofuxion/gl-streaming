/*
Copyright (c) 2024, The Khronos Group Inc.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glserver.h"
#include <GLES3/gl32.h>

// GLES 3.0
void glse_glReadBuffer()
{
    GLSE_SET_COMMAND_PTR(c, glReadBuffer);
    glReadBuffer(c->src);
}

void glse_glDrawRangeElements()
{
    GLSE_SET_COMMAND_PTR(c, glDrawRangeElements);
    if (c->indices_isnull == TRUE) {
        glDrawRangeElements(c->mode, c->start, c->end, c->count, c->type, (GLvoid *) c->indices_uint);
    } else {
        glDrawRangeElements(c->mode, c->start, c->end, c->count, c->type, (const GLvoid*)c->indices);
    }
}

void glse_glTexImage3D()
{
    GLSE_SET_COMMAND_PTR(c, glTexImage3D);
    glTexImage3D(c->target, c->level, c->internalformat, c->width, c->height, c->depth, c->border, c->format, c->type, c->pixels_isnull ? NULL : c->pixels);
}

void glse_glTexSubImage3D()
{
    GLSE_SET_COMMAND_PTR(c, glTexSubImage3D);
    glTexSubImage3D(c->target, c->level, c->xoffset, c->yoffset, c->zoffset, c->width, c->height, c->depth, c->format, c->type, c->pixels_isnull ? NULL : c->pixels);
}

void glse_glCopyTexSubImage3D()
{
    GLSE_SET_COMMAND_PTR(c, glCopyTexSubImage3D);
    glCopyTexSubImage3D(c->target, c->level, c->xoffset, c->yoffset, c->zoffset, c->x, c->y, c->width, c->height);
}

void glse_glCompressedTexImage3D()
{
    GLSE_SET_COMMAND_PTR(c, glCompressedTexImage3D);
    glCompressedTexImage3D(c->target, c->level, c->internalformat, c->width, c->height, c->depth, c->border, c->imageSize, c->data);
}

void glse_glCompressedTexSubImage3D()
{
    GLSE_SET_COMMAND_PTR(c, glCompressedTexSubImage3D);
    glCompressedTexSubImage3D(c->target, c->level, c->xoffset, c->yoffset, c->zoffset, c->width, c->height, c->depth, c->format, c->imageSize, c->data);
}

void glse_glGenQueries()
{
    GLSE_SET_COMMAND_PTR(c, glGenQueries);
    glGenQueries(c->n, (GLuint*)glsec_global.tmp_buf.buf);
    uint32_t size = c->n * sizeof(uint32_t);
    glse_cmd_send_data(0, size, (char *)glsec_global.tmp_buf.buf);
}

void glse_glDeleteQueries()
{
    GLSE_SET_COMMAND_PTR(c, glDeleteQueries);
    glDeleteQueries(c->n, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glIsQuery()
{
    GLSE_SET_COMMAND_PTR(c, glIsQuery);
    GLboolean result = glIsQuery(c->id);
    gls_ret_glIsQuery_t *ret = (gls_ret_glIsQuery_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glIsQuery;
    ret->result = result;
    glse_cmd_send_data(0, sizeof(gls_ret_glIsQuery_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glBeginQuery()
{
    GLSE_SET_COMMAND_PTR(c, glBeginQuery);
    glBeginQuery(c->target, c->id);
}

void glse_glEndQuery()
{
    GLSE_SET_COMMAND_PTR(c, glEndQuery);
    glEndQuery(c->target);
}

void glse_glGetQueryiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetQueryiv);
    gls_ret_glGetQueryiv_t *ret = (gls_ret_glGetQueryiv_t *)glsec_global.tmp_buf.buf;
    glGetQueryiv(c->target, c->pname, ret->params);
    ret->cmd = GLSC_glGetQueryiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetQueryiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetQueryObjectuiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetQueryObjectuiv);
    gls_ret_glGetQueryObjectuiv_t *ret = (gls_ret_glGetQueryObjectuiv_t *)glsec_global.tmp_buf.buf;
    glGetQueryObjectuiv(c->id, c->pname, (GLuint *)ret->params);
    ret->cmd = GLSC_glGetQueryObjectuiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetQueryObjectuiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glUnmapBuffer()
{
    GLSE_SET_COMMAND_PTR(c, glUnmapBuffer);
    GLboolean success = glUnmapBuffer(c->target);
    gls_ret_glUnmapBuffer_t *ret = (gls_ret_glUnmapBuffer_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glUnmapBuffer;
    ret->success = success;
    glse_cmd_send_data(0, sizeof(gls_ret_glUnmapBuffer_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetBufferPointerv()
{
    // This function is tricky to implement correctly as it returns a pointer.
    // For now, we'll just log an error.
    LOGE("Error: glGetBufferPointerv is not yet implemented.");
}

void glse_glDrawBuffers()
{
    GLSE_SET_COMMAND_PTR(c, glDrawBuffers);
    glDrawBuffers(c->n, (const GLenum *)glsec_global.tmp_buf.buf);
}

void glse_glUniformMatrix2x3fv()
{
    GLSE_SET_COMMAND_PTR(c, glUniformMatrix2x3fv);
    glUniformMatrix2x3fv(c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glUniformMatrix3x2fv()
{
    GLSE_SET_COMMAND_PTR(c, glUniformMatrix3x2fv);
    glUniformMatrix3x2fv(c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glUniformMatrix2x4fv()
{
    GLSE_SET_COMMAND_PTR(c, glUniformMatrix2x4fv);
    glUniformMatrix2x4fv(c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glUniformMatrix4x2fv()
{
    GLSE_SET_COMMAND_PTR(c, glUniformMatrix4x2fv);
    glUniformMatrix4x2fv(c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glUniformMatrix3x4fv()
{
    GLSE_SET_COMMAND_PTR(c, glUniformMatrix3x4fv);
    glUniformMatrix3x4fv(c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glUniformMatrix4x3fv()
{
    GLSE_SET_COMMAND_PTR(c, glUniformMatrix4x3fv);
    glUniformMatrix4x3fv(c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glBlitFramebuffer()
{
    GLSE_SET_COMMAND_PTR(c, glBlitFramebuffer);
    glBlitFramebuffer(c->srcX0, c->srcY0, c->srcX1, c->srcY1, c->dstX0, c->dstY0, c->dstX1, c->dstY1, c->mask, c->filter);
}

void glse_glRenderbufferStorageMultisample()
{
    GLSE_SET_COMMAND_PTR(c, glRenderbufferStorageMultisample);
    glRenderbufferStorageMultisample(c->target, c->samples, c->internalformat, c->width, c->height);
}

void glse_glFramebufferTextureLayer()
{
    GLSE_SET_COMMAND_PTR(c, glFramebufferTextureLayer);
    glFramebufferTextureLayer(c->target, c->attachment, c->texture, c->level, c->layer);
}

void glse_glMapBufferRange()
{
    GLSE_SET_COMMAND_PTR(c, glMapBufferRange);
    void* mapped_buffer = glMapBufferRange(c->target, c->offset, c->length, c->access);
    glse_cmd_send_data(0, c->length, mapped_buffer);
}

void glse_glFlushMappedBufferRange()
{
    GLSE_SET_COMMAND_PTR(c, glFlushMappedBufferRange);
    glFlushMappedBufferRange(c->target, c->offset, c->length);
}

void glse_glBindVertexArray()
{
    GLSE_SET_COMMAND_PTR(c, glBindVertexArray);
    glBindVertexArray(c->array);
}

void glse_glDeleteVertexArrays()
{
    GLSE_SET_COMMAND_PTR(c, glDeleteVertexArrays);
    glDeleteVertexArrays(c->n, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glGenVertexArrays()
{
    GLSE_SET_COMMAND_PTR(c, glGenVertexArrays);
    glGenVertexArrays(c->n, (GLuint*)glsec_global.tmp_buf.buf);
    uint32_t size = c->n * sizeof(uint32_t);
    glse_cmd_send_data(0, size, (char *)glsec_global.tmp_buf.buf);
}

void glse_glIsVertexArray()
{
    GLSE_SET_COMMAND_PTR(c, glIsVertexArray);
    GLboolean result = glIsVertexArray(c->array);
    gls_ret_glIsVertexArray_t *ret = (gls_ret_glIsVertexArray_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glIsVertexArray;
    ret->result = result;
    glse_cmd_send_data(0, sizeof(gls_ret_glIsVertexArray_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetIntegeri_v()
{
    GLSE_SET_COMMAND_PTR(c, glGetIntegeri_v);
    gls_ret_glGetIntegeri_v_t *ret = (gls_ret_glGetIntegeri_v_t *)glsec_global.tmp_buf.buf;
    glGetIntegerv(c->target, ret->data);
    ret->cmd = GLSC_glGetIntegeri_v;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetIntegeri_v_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glBeginTransformFeedback()
{
    GLSE_SET_COMMAND_PTR(c, glBeginTransformFeedback);
    glBeginTransformFeedback(c->primitiveMode);
}

void glse_glEndTransformFeedback()
{
    glEndTransformFeedback();
}

void glse_glBindBufferRange()
{
    GLSE_SET_COMMAND_PTR(c, glBindBufferRange);
    glBindBufferRange(c->target, c->index, c->buffer, c->offset, c->size);
}

void glse_glBindBufferBase()
{
    GLSE_SET_COMMAND_PTR(c, glBindBufferBase);
    glBindBufferBase(c->target, c->index, c->buffer);
}

void glse_glTransformFeedbackVaryings()
{
    GLSE_SET_COMMAND_PTR(c, glTransformFeedbackVaryings);
    glTransformFeedbackVaryings(c->program, c->count, (const GLchar *const*)glsec_global.tmp_buf.buf, c->bufferMode);
}

void glse_glGetTransformFeedbackVarying()
{
    GLSE_SET_COMMAND_PTR(c, glGetTransformFeedbackVarying);
    gls_ret_glGetTransformFeedbackVarying_t *ret = (gls_ret_glGetTransformFeedbackVarying_t *)glsec_global.tmp_buf.buf;
    glGetTransformFeedbackVarying(c->program, c->index, c->bufSize, &ret->length, &ret->size, &ret->type, ret->name);
    ret->cmd = GLSC_glGetTransformFeedbackVarying;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetTransformFeedbackVarying_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glVertexAttribIPointer()
{
    GLSE_SET_COMMAND_PTR(c, glVertexAttribIPointer);
    glVertexAttribIPointer(c->index, c->size, c->type, c->stride, c->pointer_isnull ? NULL : (const void*)c->pointer_uint);
}

void glse_glGetVertexAttribIiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetVertexAttribIiv);
    gls_ret_glGetVertexAttribIiv_t *ret = (gls_ret_glGetVertexAttribIiv_t *)glsec_global.tmp_buf.buf;
    glGetVertexAttribIiv(c->index, c->pname, ret->params);
    ret->cmd = GLSC_glGetVertexAttribIiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetVertexAttribIiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetVertexAttribIuiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetVertexAttribIuiv);
    gls_ret_glGetVertexAttribIuiv_t *ret = (gls_ret_glGetVertexAttribIuiv_t *)glsec_global.tmp_buf.buf;
    glGetVertexAttribIuiv(c->index, c->pname, (GLuint *)ret->params);
    ret->cmd = GLSC_glGetVertexAttribIuiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetVertexAttribIuiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glVertexAttribI4i()
{
    GLSE_SET_COMMAND_PTR(c, glVertexAttribI4i);
    glVertexAttribI4i(c->index, c->x, c->y, c->z, c->w);
}

void glse_glVertexAttribI4ui()
{
    GLSE_SET_COMMAND_PTR(c, glVertexAttribI4ui);
    glVertexAttribI4ui(c->index, c->x, c->y, c->z, c->w);
}

void glse_glVertexAttribI4iv()
{
    GLSE_SET_COMMAND_PTR(c, glVertexAttribI4iv);
    glVertexAttribI4iv(c->index, (const GLint *)glsec_global.tmp_buf.buf);
}

void glse_glVertexAttribI4uiv()
{
    GLSE_SET_COMMAND_PTR(c, glVertexAttribI4uiv);
    glVertexAttribI4uiv(c->index, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glGetUniformuiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetUniformuiv);
    gls_ret_glGetUniformuiv_t *ret = (gls_ret_glGetUniformuiv_t *)glsec_global.tmp_buf.buf;
    glGetUniformuiv(c->program, c->location, (GLuint *)ret->params);
    ret->cmd = GLSC_glGetUniformuiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetUniformuiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetFragDataLocation()
{
    GLSE_SET_COMMAND_PTR(c, glGetFragDataLocation);
    GLint location = glGetFragDataLocation(c->program, c->name);
    gls_ret_glGetFragDataLocation_t *ret = (gls_ret_glGetFragDataLocation_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glGetFragDataLocation;
    ret->location = location;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetFragDataLocation_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glUniform1ui()
{
    GLSE_SET_COMMAND_PTR(c, glUniform1ui);
    glUniform1ui(c->location, c->v0);
}

void glse_glUniform2ui()
{
    GLSE_SET_COMMAND_PTR(c, glUniform2ui);
    glUniform2ui(c->location, c->v0, c->v1);
}

void glse_glUniform3ui()
{
    GLSE_SET_COMMAND_PTR(c, glUniform3ui);
    glUniform3ui(c->location, c->v0, c->v1, c->v2);
}

void glse_glUniform4ui()
{
    GLSE_SET_COMMAND_PTR(c, glUniform4ui);
    glUniform4ui(c->location, c->v0, c->v1, c->v2, c->v3);
}

void glse_glUniform1uiv()
{
    GLSE_SET_COMMAND_PTR(c, glUniform1uiv);
    glUniform1uiv(c->location, c->count, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glUniform2uiv()
{
    GLSE_SET_COMMAND_PTR(c, glUniform2uiv);
    glUniform2uiv(c->location, c->count, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glUniform3uiv()
{
    GLSE_SET_COMMAND_PTR(c, glUniform3uiv);
    glUniform3uiv(c->location, c->count, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glUniform4uiv()
{
    GLSE_SET_COMMAND_PTR(c, glUniform4uiv);
    glUniform4uiv(c->location, c->count, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glClearBufferiv()
{
    GLSE_SET_COMMAND_PTR(c, glClearBufferiv);
    glClearBufferiv(c->buffer, c->drawbuffer, (const GLint *)glsec_global.tmp_buf.buf);
}

void glse_glClearBufferuiv()
{
    GLSE_SET_COMMAND_PTR(c, glClearBufferuiv);
    glClearBufferuiv(c->buffer, c->drawbuffer, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glClearBufferfv()
{
    GLSE_SET_COMMAND_PTR(c, glClearBufferfv);
    glClearBufferfv(c->buffer, c->drawbuffer, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glClearBufferfi()
{
    GLSE_SET_COMMAND_PTR(c, glClearBufferfi);
    glClearBufferfi(c->buffer, c->drawbuffer, c->depth, c->stencil);
}

void glse_glGetStringi()
{
    GLSE_SET_COMMAND_PTR(c, glGetStringi);
    const GLubyte *params = glGetStringi(c->name, c->index);
    gls_ret_glGetStringi_t *ret = (gls_ret_glGetStringi_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glGetStringi;
    strncpy(ret->params, (const char*)params, GLS_STRING_SIZE);
    glse_cmd_send_data(0, sizeof(gls_ret_glGetStringi_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glCopyBufferSubData()
{
    GLSE_SET_COMMAND_PTR(c, glCopyBufferSubData);
    glCopyBufferSubData(c->readTarget, c->writeTarget, c->readOffset, c->writeOffset, c->size);
}

void glse_glGetUniformIndices()
{
    GLSE_SET_COMMAND_PTR(c, glGetUniformIndices);
    gls_ret_glGetUniformIndices_t *ret = (gls_ret_glGetUniformIndices_t *)glsec_global.tmp_buf.buf;
    glGetUniformIndices(c->program, c->uniformCount, (const GLchar *const*)glsec_global.tmp_buf.buf, ret->uniformIndices);
    ret->cmd = GLSC_glGetUniformIndices;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetUniformIndices_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetActiveUniformsiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetActiveUniformsiv);
    gls_ret_glGetActiveUniformsiv_t *ret = (gls_ret_glGetActiveUniformsiv_t *)glsec_global.tmp_buf.buf;
    glGetActiveUniformsiv(c->program, c->uniformCount, (const GLuint *)glsec_global.tmp_buf.buf, c->pname, ret->params);
    ret->cmd = GLSC_glGetActiveUniformsiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetActiveUniformsiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetUniformBlockIndex()
{
    GLSE_SET_COMMAND_PTR(c, glGetUniformBlockIndex);
    GLuint blockIndex = glGetUniformBlockIndex(c->program, c->uniformBlockName);
    gls_ret_glGetUniformBlockIndex_t *ret = (gls_ret_glGetUniformBlockIndex_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glGetUniformBlockIndex;
    ret->uniformBlockIndex = blockIndex;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetUniformBlockIndex_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetActiveUniformBlockiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetActiveUniformBlockiv);
    gls_ret_glGetActiveUniformBlockiv_t *ret = (gls_ret_glGetActiveUniformBlockiv_t *)glsec_global.tmp_buf.buf;
    glGetActiveUniformBlockiv(c->program, c->uniformBlockIndex, c->pname, ret->params);
    ret->cmd = GLSC_glGetActiveUniformBlockiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetActiveUniformBlockiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetActiveUniformBlockName()
{
    GLSE_SET_COMMAND_PTR(c, glGetActiveUniformBlockName);
    gls_ret_glGetActiveUniformBlockName_t *ret = (gls_ret_glGetActiveUniformBlockName_t *)glsec_global.tmp_buf.buf;
    glGetActiveUniformBlockName(c->program, c->uniformBlockIndex, c->bufSize, &ret->length, ret->uniformBlockName);
    ret->cmd = GLSC_glGetActiveUniformBlockName;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetActiveUniformBlockName_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glUniformBlockBinding()
{
    GLSE_SET_COMMAND_PTR(c, glUniformBlockBinding);
    glUniformBlockBinding(c->program, c->uniformBlockIndex, c->uniformBlockBinding);
}

void glse_glDrawArraysInstanced()
{
    GLSE_SET_COMMAND_PTR(c, glDrawArraysInstanced);
    glDrawArraysInstanced(c->mode, c->first, c->count, c->instancecount);
}

void glse_glDrawElementsInstanced()
{
    GLSE_SET_COMMAND_PTR(c, glDrawElementsInstanced);
    glDrawElementsInstanced(c->mode, c->count, c->type, (const void *)c->indices_uint, c->instancecount);
}

void glse_glFenceSync()
{
    GLSE_SET_COMMAND_PTR(c, glFenceSync);
    GLsync sync = glFenceSync(c->condition, c->flags);
    gls_ret_glFenceSync_t *ret = (gls_ret_glFenceSync_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glFenceSync;
    //ret->sync = sync; // FIXME: How to handle pointer types?
    glse_cmd_send_data(0, sizeof(gls_ret_glFenceSync_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glIsSync()
{
    GLSE_SET_COMMAND_PTR(c, glIsSync);
    //GLboolean result = glIsSync(c->sync);
    gls_ret_glIsSync_t *ret = (gls_ret_glIsSync_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glIsSync;
    //ret->result = result;
    glse_cmd_send_data(0, sizeof(gls_ret_glIsSync_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glDeleteSync()
{
    GLSE_SET_COMMAND_PTR(c, glDeleteSync);
    //glDeleteSync(c->sync);
}

void glse_glClientWaitSync()
{
    GLSE_SET_COMMAND_PTR(c, glClientWaitSync);
    //GLenum result = glClientWaitSync(c->sync, c->flags, c->timeout);
    gls_ret_glClientWaitSync_t *ret = (gls_ret_glClientWaitSync_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glClientWaitSync;
    //ret->result = result;
    glse_cmd_send_data(0, sizeof(gls_ret_glClientWaitSync_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glWaitSync()
{
    GLSE_SET_COMMAND_PTR(c, glWaitSync);
    //glWaitSync(c->sync, c->flags, c->timeout);
}

void glse_glGetInteger64v()
{
    GLSE_SET_COMMAND_PTR(c, glGetInteger64v);
    gls_ret_glGetInteger64v_t *ret = (gls_ret_glGetInteger64v_t *)glsec_global.tmp_buf.buf;
    glGetInteger64v(c->pname, (GLint64 *)ret->data);
    ret->cmd = GLSC_glGetInteger64v;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetInteger64v_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetSynciv()
{
    GLSE_SET_COMMAND_PTR(c, glGetSynciv);
    gls_ret_glGetSynciv_t *ret = (gls_ret_glGetSynciv_t *)glsec_global.tmp_buf.buf;
    //glGetSynciv(c->sync, c->pname, c->bufSize, &ret->length, ret->values);
    ret->cmd = GLSC_glGetSynciv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetSynciv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetInteger64i_v()
{
    GLSE_SET_COMMAND_PTR(c, glGetInteger64i_v);
    gls_ret_glGetInteger64i_v_t *ret = (gls_ret_glGetInteger64i_v_t *)glsec_global.tmp_buf.buf;
    glGetInteger64i_v(c->target, c->index, (GLint64 *)ret->data);
    ret->cmd = GLSC_glGetInteger64i_v;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetInteger64i_v_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetBufferParameteri64v()
{
    GLSE_SET_COMMAND_PTR(c, glGetBufferParameteri64v);
    gls_ret_glGetBufferParameteri64v_t *ret = (gls_ret_glGetBufferParameteri64v_t *)glsec_global.tmp_buf.buf;
    glGetBufferParameteri64v(c->target, c->pname, (GLint64 *)ret->params);
    ret->cmd = GLSC_glGetBufferParameteri64v;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetBufferParameteri64v_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGenSamplers()
{
    GLSE_SET_COMMAND_PTR(c, glGenSamplers);
    glGenSamplers(c->count, (GLuint *)glsec_global.tmp_buf.buf);
    uint32_t size = c->count * sizeof(uint32_t);
    glse_cmd_send_data(0, size, (char *)glsec_global.tmp_buf.buf);
}

void glse_glDeleteSamplers()
{
    GLSE_SET_COMMAND_PTR(c, glDeleteSamplers);
    glDeleteSamplers(c->count, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glIsSampler()
{
    GLSE_SET_COMMAND_PTR(c, glIsSampler);
    GLboolean result = glIsSampler(c->sampler);
    gls_ret_glIsSampler_t *ret = (gls_ret_glIsSampler_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glIsSampler;
    ret->result = result;
    glse_cmd_send_data(0, sizeof(gls_ret_glIsSampler_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glBindSampler()
{
    GLSE_SET_COMMAND_PTR(c, glBindSampler);
    glBindSampler(c->unit, c->sampler);
}

void glse_glSamplerParameteri()
{
    GLSE_SET_COMMAND_PTR(c, glSamplerParameteri);
    glSamplerParameteri(c->sampler, c->pname, c->param);
}

void glse_glSamplerParameteriv()
{
    GLSE_SET_COMMAND_PTR(c, glSamplerParameteriv);
    glSamplerParameteriv(c->sampler, c->pname, (const GLint *)glsec_global.tmp_buf.buf);
}

void glse_glSamplerParameterf()
{
    GLSE_SET_COMMAND_PTR(c, glSamplerParameterf);
    glSamplerParameterf(c->sampler, c->pname, c->param);
}

void glse_glSamplerParameterfv()
{
    GLSE_SET_COMMAND_PTR(c, glSamplerParameterfv);
    glSamplerParameterfv(c->sampler, c->pname, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glGetSamplerParameteriv()
{
    GLSE_SET_COMMAND_PTR(c, glGetSamplerParameteriv);
    gls_ret_glGetSamplerParameteriv_t *ret = (gls_ret_glGetSamplerParameteriv_t *)glsec_global.tmp_buf.buf;
    glGetSamplerParameteriv(c->sampler, c->pname, ret->params);
    ret->cmd = GLSC_glGetSamplerParameteriv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetSamplerParameteriv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetSamplerParameterfv()
{
    GLSE_SET_COMMAND_PTR(c, glGetSamplerParameterfv);
    gls_ret_glGetSamplerParameterfv_t *ret = (gls_ret_glGetSamplerParameterfv_t *)glsec_global.tmp_buf.buf;
    glGetSamplerParameterfv(c->sampler, c->pname, ret->params);
    ret->cmd = GLSC_glGetSamplerParameterfv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetSamplerParameterfv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glVertexAttribDivisor()
{
    GLSE_SET_COMMAND_PTR(c, glVertexAttribDivisor);
    glVertexAttribDivisor(c->index, c->divisor);
}

void glse_glBindTransformFeedback()
{
    GLSE_SET_COMMAND_PTR(c, glBindTransformFeedback);
    glBindTransformFeedback(c->target, c->id);
}

void glse_glDeleteTransformFeedbacks()
{
    GLSE_SET_COMMAND_PTR(c, glDeleteTransformFeedbacks);
    glDeleteTransformFeedbacks(c->n, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glGenTransformFeedbacks()
{
    GLSE_SET_COMMAND_PTR(c, glGenTransformFeedbacks);
    glGenTransformFeedbacks(c->n, (GLuint *)glsec_global.tmp_buf.buf);
    uint32_t size = c->n * sizeof(uint32_t);
    glse_cmd_send_data(0, size, (char *)glsec_global.tmp_buf.buf);
}

void glse_glIsTransformFeedback()
{
    GLSE_SET_COMMAND_PTR(c, glIsTransformFeedback);
    GLboolean result = glIsTransformFeedback(c->id);
    gls_ret_glIsTransformFeedback_t *ret = (gls_ret_glIsTransformFeedback_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glIsTransformFeedback;
    ret->result = result;
    glse_cmd_send_data(0, sizeof(gls_ret_glIsTransformFeedback_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glPauseTransformFeedback()
{
    glPauseTransformFeedback();
}

void glse_glResumeTransformFeedback()
{
    glResumeTransformFeedback();
}

void glse_glGetProgramBinary()
{
    GLSE_SET_COMMAND_PTR(c, glGetProgramBinary);
    gls_ret_glGetProgramBinary_t *ret = (gls_ret_glGetProgramBinary_t *)glsec_global.tmp_buf.buf;
    glGetProgramBinary(c->program, c->bufSize, &ret->length, &ret->binaryFormat, glsec_global.tmp_buf.buf + sizeof(gls_ret_glGetProgramBinary_t));
    ret->cmd = GLSC_glGetProgramBinary;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetProgramBinary_t) + ret->length, (char *)glsec_global.tmp_buf.buf);
}

void glse_glProgramBinary()
{
    GLSE_SET_COMMAND_PTR(c, glProgramBinary);
    glProgramBinary(c->program, c->binaryFormat, glsec_global.tmp_buf.buf, c->length);
}

void glse_glProgramParameteri()
{
    GLSE_SET_COMMAND_PTR(c, glProgramParameteri);
    glProgramParameteri(c->program, c->pname, c->value);
}

void glse_glInvalidateFramebuffer()
{
    GLSE_SET_COMMAND_PTR(c, glInvalidateFramebuffer);
    glInvalidateFramebuffer(c->target, c->numAttachments, (const GLenum *)glsec_global.tmp_buf.buf);
}

void glse_glInvalidateSubFramebuffer()
{
    GLSE_SET_COMMAND_PTR(c, glInvalidateSubFramebuffer);
    glInvalidateSubFramebuffer(c->target, c->numAttachments, (const GLenum *)glsec_global.tmp_buf.buf, c->x, c->y, c->width, c->height);
}

void glse_glTexStorage2D()
{
    GLSE_SET_COMMAND_PTR(c, glTexStorage2D);
    glTexStorage2D(c->target, c->levels, c->internalformat, c->width, c->height);
}

void glse_glTexStorage3D()
{
    GLSE_SET_COMMAND_PTR(c, glTexStorage3D);
    glTexStorage3D(c->target, c->levels, c->internalformat, c->width, c->height, c->depth);
}

void glse_glGetInternalformativ()
{
    GLSE_SET_COMMAND_PTR(c, glGetInternalformativ);
    gls_ret_glGetInternalformativ_t *ret = (gls_ret_glGetInternalformativ_t *)glsec_global.tmp_buf.buf;
    glGetInternalformativ(c->target, c->internalformat, c->pname, c->bufSize, ret->params);
    ret->cmd = GLSC_glGetInternalformativ;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetInternalformativ_t), (char *)glsec_global.tmp_buf.buf);
}

// GLES 3.1
void glse_glDispatchCompute()
{
    GLSE_SET_COMMAND_PTR(c, glDispatchCompute);
    glDispatchCompute(c->num_groups_x, c->num_groups_y, c->num_groups_z);
}

void glse_glDispatchComputeIndirect()
{
    GLSE_SET_COMMAND_PTR(c, glDispatchComputeIndirect);
    glDispatchComputeIndirect(c->indirect);
}

void glse_glDrawArraysIndirect()
{
    GLSE_SET_COMMAND_PTR(c, glDrawArraysIndirect);
    glDrawArraysIndirect(c->mode, (const void *)c->indirect_uint);
}

void glse_glDrawElementsIndirect()
{
    GLSE_SET_COMMAND_PTR(c, glDrawElementsIndirect);
    glDrawElementsIndirect(c->mode, c->type, (const void *)c->indirect_uint);
}

void glse_glFramebufferParameteri()
{
    GLSE_SET_COMMAND_PTR(c, glFramebufferParameteri);
    glFramebufferParameteri(c->target, c->pname, c->param);
}

void glse_glGetFramebufferParameteriv()
{
    GLSE_SET_COMMAND_PTR(c, glGetFramebufferParameteriv);
    gls_ret_glGetFramebufferParameteriv_t *ret = (gls_ret_glGetFramebufferParameteriv_t *)glsec_global.tmp_buf.buf;
    glGetFramebufferParameteriv(c->target, c->pname, ret->params);
    ret->cmd = GLSC_glGetFramebufferParameteriv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetFramebufferParameteriv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetProgramInterfaceiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetProgramInterfaceiv);
    gls_ret_glGetProgramInterfaceiv_t *ret = (gls_ret_glGetProgramInterfaceiv_t *)glsec_global.tmp_buf.buf;
    glGetProgramInterfaceiv(c->program, c->programInterface, c->pname, ret->params);
    ret->cmd = GLSC_glGetProgramInterfaceiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetProgramInterfaceiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetProgramResourceIndex()
{
    GLSE_SET_COMMAND_PTR(c, glGetProgramResourceIndex);
    GLuint index = glGetProgramResourceIndex(c->program, c->programInterface, c->name);
    gls_ret_glGetProgramResourceIndex_t *ret = (gls_ret_glGetProgramResourceIndex_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glGetProgramResourceIndex;
    ret->index = index;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetProgramResourceIndex_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetProgramResourceName()
{
    GLSE_SET_COMMAND_PTR(c, glGetProgramResourceName);
    gls_ret_glGetProgramResourceName_t *ret = (gls_ret_glGetProgramResourceName_t *)glsec_global.tmp_buf.buf;
    glGetProgramResourceName(c->program, c->programInterface, c->index, c->bufSize, &ret->length, ret->name);
    ret->cmd = GLSC_glGetProgramResourceName;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetProgramResourceName_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetProgramResourceiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetProgramResourceiv);
    gls_ret_glGetProgramResourceiv_t *ret = (gls_ret_glGetProgramResourceiv_t *)glsec_global.tmp_buf.buf;
    glGetProgramResourceiv(c->program, c->programInterface, c->index, c->propCount, (const GLenum *)glsec_global.tmp_buf.buf, c->bufSize, &ret->length, ret->params);
    ret->cmd = GLSC_glGetProgramResourceiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetProgramResourceiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetProgramResourceLocation()
{
    GLSE_SET_COMMAND_PTR(c, glGetProgramResourceLocation);
    GLint location = glGetProgramResourceLocation(c->program, c->programInterface, c->name);
    gls_ret_glGetProgramResourceLocation_t *ret = (gls_ret_glGetProgramResourceLocation_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glGetProgramResourceLocation;
    ret->location = location;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetProgramResourceLocation_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glUseProgramStages()
{
    GLSE_SET_COMMAND_PTR(c, glUseProgramStages);
    glUseProgramStages(c->pipeline, c->stages, c->program);
}

void glse_glActiveShaderProgram()
{
    GLSE_SET_COMMAND_PTR(c, glActiveShaderProgram);
    glActiveShaderProgram(c->pipeline, c->program);
}

void glse_glCreateShaderProgramv()
{
    GLSE_SET_COMMAND_PTR(c, glCreateShaderProgramv);
    GLuint program = glCreateShaderProgramv(c->type, c->count, (const GLchar *const*)glsec_global.tmp_buf.buf);
    gls_ret_glCreateShaderProgramv_t *ret = (gls_ret_glCreateShaderProgramv_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glCreateShaderProgramv;
    ret->program = program;
    glse_cmd_send_data(0, sizeof(gls_ret_glCreateShaderProgramv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glBindProgramPipeline()
{
    GLSE_SET_COMMAND_PTR(c, glBindProgramPipeline);
    glBindProgramPipeline(c->pipeline);
}

void glse_glDeleteProgramPipelines()
{
    GLSE_SET_COMMAND_PTR(c, glDeleteProgramPipelines);
    glDeleteProgramPipelines(c->n, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glGenProgramPipelines()
{
    GLSE_SET_COMMAND_PTR(c, glGenProgramPipelines);
    glGenProgramPipelines(c->n, (GLuint *)glsec_global.tmp_buf.buf);
    uint32_t size = c->n * sizeof(uint32_t);
    glse_cmd_send_data(0, size, (char *)glsec_global.tmp_buf.buf);
}

void glse_glIsProgramPipeline()
{
    GLSE_SET_COMMAND_PTR(c, glIsProgramPipeline);
    GLboolean result = glIsProgramPipeline(c->pipeline);
    gls_ret_glIsProgramPipeline_t *ret = (gls_ret_glIsProgramPipeline_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glIsProgramPipeline;
    ret->result = result;
    glse_cmd_send_data(0, sizeof(gls_ret_glIsProgramPipeline_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetProgramPipelineiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetProgramPipelineiv);
    gls_ret_glGetProgramPipelineiv_t *ret = (gls_ret_glGetProgramPipelineiv_t *)glsec_global.tmp_buf.buf;
    glGetProgramPipelineiv(c->pipeline, c->pname, ret->params);
    ret->cmd = GLSC_glGetProgramPipelineiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetProgramPipelineiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniform1i()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform1i);
    glProgramUniform1i(c->program, c->location, c->v0);
}

void glse_glProgramUniform2i()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform2i);
    glProgramUniform2i(c->program, c->location, c->v0, c->v1);
}

void glse_glProgramUniform3i()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform3i);
    glProgramUniform3i(c->program, c->location, c->v0, c->v1, c->v2);
}

void glse_glProgramUniform4i()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform4i);
    glProgramUniform4i(c->program, c->location, c->v0, c->v1, c->v2, c->v3);
}

void glse_glProgramUniform1ui()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform1ui);
    glProgramUniform1ui(c->program, c->location, c->v0);
}

void glse_glProgramUniform2ui()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform2ui);
    glProgramUniform2ui(c->program, c->location, c->v0, c->v1);
}

void glse_glProgramUniform3ui()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform3ui);
    glProgramUniform3ui(c->program, c->location, c->v0, c->v1, c->v2);
}

void glse_glProgramUniform4ui()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform4ui);
    glProgramUniform4ui(c->program, c->location, c->v0, c->v1, c->v2, c->v3);
}

void glse_glProgramUniform1f()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform1f);
    glProgramUniform1f(c->program, c->location, c->v0);
}

void glse_glProgramUniform2f()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform2f);
    glProgramUniform2f(c->program, c->location, c->v0, c->v1);
}

void glse_glProgramUniform3f()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform3f);
    glProgramUniform3f(c->program, c->location, c->v0, c->v1, c->v2);
}

void glse_glProgramUniform4f()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform4f);
    glProgramUniform4f(c->program, c->location, c->v0, c->v1, c->v2, c->v3);
}

void glse_glProgramUniform1iv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform1iv);
    glProgramUniform1iv(c->program, c->location, c->count, (const GLint *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniform2iv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform2iv);
    glProgramUniform2iv(c->program, c->location, c->count, (const GLint *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniform3iv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform3iv);
    glProgramUniform3iv(c->program, c->location, c->count, (const GLint *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniform4iv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform4iv);
    glProgramUniform4iv(c->program, c->location, c->count, (const GLint *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniform1uiv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform1uiv);
    glProgramUniform1uiv(c->program, c->location, c->count, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniform2uiv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform2uiv);
    glProgramUniform2uiv(c->program, c->location, c->count, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniform3uiv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform3uiv);
    glProgramUniform3uiv(c->program, c->location, c->count, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniform4uiv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform4uiv);
    glProgramUniform4uiv(c->program, c->location, c->count, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniform1fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform1fv);
    glProgramUniform1fv(c->program, c->location, c->count, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniform2fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform2fv);
    glProgramUniform2fv(c->program, c->location, c->count, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniform3fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform3fv);
    glProgramUniform3fv(c->program, c->location, c->count, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniform4fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniform4fv);
    glProgramUniform4fv(c->program, c->location, c->count, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniformMatrix2fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniformMatrix2fv);
    glProgramUniformMatrix2fv(c->program, c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniformMatrix3fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniformMatrix3fv);
    glProgramUniformMatrix3fv(c->program, c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniformMatrix4fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniformMatrix4fv);
    glProgramUniformMatrix4fv(c->program, c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniformMatrix2x3fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniformMatrix2x3fv);
    glProgramUniformMatrix2x3fv(c->program, c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniformMatrix3x2fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniformMatrix3x2fv);
    glProgramUniformMatrix3x2fv(c->program, c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniformMatrix2x4fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniformMatrix2x4fv);
    glProgramUniformMatrix2x4fv(c->program, c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniformMatrix4x2fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniformMatrix4x2fv);
    glProgramUniformMatrix4x2fv(c->program, c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniformMatrix3x4fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniformMatrix3x4fv);
    glProgramUniformMatrix3x4fv(c->program, c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glProgramUniformMatrix4x3fv()
{
    GLSE_SET_COMMAND_PTR(c, glProgramUniformMatrix4x3fv);
    glProgramUniformMatrix4x3fv(c->program, c->location, c->count, c->transpose, (const GLfloat *)glsec_global.tmp_buf.buf);
}

void glse_glValidateProgramPipeline()
{
    GLSE_SET_COMMAND_PTR(c, glValidateProgramPipeline);
    glValidateProgramPipeline(c->pipeline);
}

void glse_glGetProgramPipelineInfoLog()
{
    GLSE_SET_COMMAND_PTR(c, glGetProgramPipelineInfoLog);
    gls_ret_glGetProgramPipelineInfoLog_t *ret = (gls_ret_glGetProgramPipelineInfoLog_t *)glsec_global.tmp_buf.buf;
    glGetProgramPipelineInfoLog(c->pipeline, c->bufSize, &ret->length, ret->infoLog);
    ret->cmd = GLSC_glGetProgramPipelineInfoLog;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetProgramPipelineInfoLog_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glBindImageTexture()
{
    GLSE_SET_COMMAND_PTR(c, glBindImageTexture);
    glBindImageTexture(c->unit, c->texture, c->level, c->layered, c->layer, c->access, c->format);
}

void glse_glGetBooleani_v()
{
    GLSE_SET_COMMAND_PTR(c, glGetBooleani_v);
    gls_ret_glGetBooleani_v_t *ret = (gls_ret_glGetBooleani_v_t *)glsec_global.tmp_buf.buf;
    glGetBooleani_v(c->target, c->index, (GLboolean *)&ret->data);
    ret->cmd = GLSC_glGetBooleani_v;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetBooleani_v_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glMemoryBarrier()
{
    GLSE_SET_COMMAND_PTR(c, glMemoryBarrier);
    glMemoryBarrier(c->barriers);
}

void glse_glMemoryBarrierByRegion()
{
    GLSE_SET_COMMAND_PTR(c, glMemoryBarrierByRegion);
    glMemoryBarrierByRegion(c->barriers);
}

void glse_glTexStorage2DMultisample()
{
    GLSE_SET_COMMAND_PTR(c, glTexStorage2DMultisample);
    glTexStorage2DMultisample(c->target, c->samples, c->internalformat, c->width, c->height, c->fixedsamplelocations);
}

void glse_glGetMultisamplefv()
{
    GLSE_SET_COMMAND_PTR(c, glGetMultisamplefv);
    gls_ret_glGetMultisamplefv_t *ret = (gls_ret_glGetMultisamplefv_t *)glsec_global.tmp_buf.buf;
    glGetMultisamplefv(c->pname, c->index, ret->val);
    ret->cmd = GLSC_glGetMultisamplefv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetMultisamplefv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glSampleMaski()
{
    GLSE_SET_COMMAND_PTR(c, glSampleMaski);
    glSampleMaski(c->maskNumber, c->mask);
}

void glse_glGetTexLevelParameteriv()
{
    GLSE_SET_COMMAND_PTR(c, glGetTexLevelParameteriv);
    gls_ret_glGetTexLevelParameteriv_t *ret = (gls_ret_glGetTexLevelParameteriv_t *)glsec_global.tmp_buf.buf;
    glGetTexLevelParameteriv(c->target, c->level, c->pname, ret->params);
    ret->cmd = GLSC_glGetTexLevelParameteriv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetTexLevelParameteriv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetTexLevelParameterfv()
{
    GLSE_SET_COMMAND_PTR(c, glGetTexLevelParameterfv);
    gls_ret_glGetTexLevelParameterfv_t *ret = (gls_ret_glGetTexLevelParameterfv_t *)glsec_global.tmp_buf.buf;
    glGetTexLevelParameterfv(c->target, c->level, c->pname, ret->params);
    ret->cmd = GLSC_glGetTexLevelParameterfv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetTexLevelParameterfv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glBindVertexBuffer()
{
    GLSE_SET_COMMAND_PTR(c, glBindVertexBuffer);
    glBindVertexBuffer(c->bindingindex, c->buffer, c->offset, c->stride);
}

void glse_glVertexAttribFormat()
{
    GLSE_SET_COMMAND_PTR(c, glVertexAttribFormat);
    glVertexAttribFormat(c->attribindex, c->size, c->type, c->normalized, c->relativeoffset);
}

void glse_glVertexAttribIFormat()
{
    GLSE_SET_COMMAND_PTR(c, glVertexAttribIFormat);
    glVertexAttribIFormat(c->attribindex, c->size, c->type, c->relativeoffset);
}

void glse_glVertexAttribBinding()
{
    GLSE_SET_COMMAND_PTR(c, glVertexAttribBinding);
    glVertexAttribBinding(c->attribindex, c->bindingindex);
}

void glse_glVertexBindingDivisor()
{
    GLSE_SET_COMMAND_PTR(c, glVertexBindingDivisor);
    glVertexBindingDivisor(c->bindingindex, c->divisor);
}

// GLES 3.2
void glse_glBlendBarrier()
{
    glBlendBarrier();
}

void glse_glCopyImageSubData()
{
    GLSE_SET_COMMAND_PTR(c, glCopyImageSubData);
    glCopyImageSubData(c->srcName, c->srcTarget, c->srcLevel, c->srcX, c->srcY, c->srcZ, c->dstName, c->dstTarget, c->dstLevel, c->dstX, c->dstY, c->dstZ, c->srcWidth, c->srcHeight, c->srcDepth);
}

void glse_glDebugMessageControl()
{
    GLSE_SET_COMMAND_PTR(c, glDebugMessageControl);
    glDebugMessageControl(c->source, c->type, c->severity, c->count, (const GLuint *)glsec_global.tmp_buf.buf, c->enabled);
}

void glse_glDebugMessageInsert()
{
    GLSE_SET_COMMAND_PTR(c, glDebugMessageInsert);
    glDebugMessageInsert(c->source, c->type, c->id, c->severity, c->length, (const GLchar *)glsec_global.tmp_buf.buf);
}

void glse_glDebugMessageCallback()
{
    // This function is not safe to implement as it involves function pointers.
    LOGE("Error: glDebugMessageCallback is not supported.");
}

void glse_glGetDebugMessageLog()
{
    GLSE_SET_COMMAND_PTR(c, glGetDebugMessageLog);
    // This function is complex to implement due to multiple output arrays.
    LOGE("Error: glGetDebugMessageLog is not yet implemented.");
}

void glse_glPushDebugGroup()
{
    GLSE_SET_COMMAND_PTR(c, glPushDebugGroup);
    glPushDebugGroup(c->source, c->id, c->length, (const GLchar *)glsec_global.tmp_buf.buf);
}

void glse_glPopDebugGroup()
{
    glPopDebugGroup();
}

void glse_glObjectLabel()
{
    GLSE_SET_COMMAND_PTR(c, glObjectLabel);
    glObjectLabel(c->identifier, c->name, c->length, (const GLchar *)glsec_global.tmp_buf.buf);
}

void glse_glGetObjectLabel()
{
    GLSE_SET_COMMAND_PTR(c, glGetObjectLabel);
    gls_ret_glGetObjectLabel_t *ret = (gls_ret_glGetObjectLabel_t *)glsec_global.tmp_buf.buf;
    glGetObjectLabel(c->identifier, c->name, c->bufSize, &ret->length, ret->label);
    ret->cmd = GLSC_glGetObjectLabel;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetObjectLabel_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glObjectPtrLabel()
{
    GLSE_SET_COMMAND_PTR(c, glObjectPtrLabel);
    // This function is not safe to implement as it involves pointers.
    LOGE("Error: glObjectPtrLabel is not supported.");
}

void glse_glGetObjectPtrLabel()
{
    GLSE_SET_COMMAND_PTR(c, glGetObjectPtrLabel);
    // This function is not safe to implement as it involves pointers.
    LOGE("Error: glGetObjectPtrLabel is not supported.");
}

void glse_glGetPointerv()
{
    // This function is not safe to implement as it involves pointers.
    LOGE("Error: glGetPointerv is not supported.");
}

void glse_glEnablei()
{
    GLSE_SET_COMMAND_PTR(c, glEnablei);
    glEnablei(c->target, c->index);
}

void glse_glDisablei()
{
    GLSE_SET_COMMAND_PTR(c, glDisablei);
    glDisablei(c->target, c->index);
}

void glse_glBlendEquationi()
{
    GLSE_SET_COMMAND_PTR(c, glBlendEquationi);
    glBlendEquationi(c->buf, c->mode);
}

void glse_glBlendEquationSeparatei()
{
    GLSE_SET_COMMAND_PTR(c, glBlendEquationSeparatei);
    glBlendEquationSeparatei(c->buf, c->modeRGB, c->modeAlpha);
}

void glse_glBlendFunci()
{
    GLSE_SET_COMMAND_PTR(c, glBlendFunci);
    glBlendFunci(c->buf, c->src, c->dst);
}

void glse_glBlendFuncSeparatei()
{
    GLSE_SET_COMMAND_PTR(c, glBlendFuncSeparatei);
    glBlendFuncSeparatei(c->buf, c->srcRGB, c->dstRGB, c->srcAlpha, c->dstAlpha);
}

void glse_glColorMaski()
{
    GLSE_SET_COMMAND_PTR(c, glColorMaski);
    glColorMaski(c->index, c->r, c->g, c->b, c->a);
}

void glse_glIsEnabledi()
{
    GLSE_SET_COMMAND_PTR(c, glIsEnabledi);
    GLboolean result = glIsEnabledi(c->target, c->index);
    gls_ret_glIsEnabledi_t *ret = (gls_ret_glIsEnabledi_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glIsEnabledi;
    ret->result = result;
    glse_cmd_send_data(0, sizeof(gls_ret_glIsEnabledi_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glDrawElementsBaseVertex()
{
    GLSE_SET_COMMAND_PTR(c, glDrawElementsBaseVertex);
    glDrawElementsBaseVertex(c->mode, c->count, c->type, (const void *)glsec_global.tmp_buf.buf, c->basevertex);
}

void glse_glDrawRangeElementsBaseVertex()
{
    GLSE_SET_COMMAND_PTR(c, glDrawRangeElementsBaseVertex);
    glDrawRangeElementsBaseVertex(c->mode, c->start, c->end, c->count, c->type, (const void *)glsec_global.tmp_buf.buf, c->basevertex);
}

void glse_glDrawElementsInstancedBaseVertex()
{
    GLSE_SET_COMMAND_PTR(c, glDrawElementsInstancedBaseVertex);
    glDrawElementsInstancedBaseVertex(c->mode, c->count, c->type, (const void *)glsec_global.tmp_buf.buf, c->instancecount, c->basevertex);
}

void glse_glFramebufferTexture()
{
    GLSE_SET_COMMAND_PTR(c, glFramebufferTexture);
    glFramebufferTexture(c->target, c->attachment, c->texture, c->level);
}

void glse_glPrimitiveBoundingBox()
{
    GLSE_SET_COMMAND_PTR(c, glPrimitiveBoundingBox);
    glPrimitiveBoundingBox(c->minX, c->minY, c->minZ, c->minW, c->maxX, c->maxY, c->maxZ, c->maxW);
}

void glse_glGetGraphicsResetStatus()
{
    GLenum status = glGetGraphicsResetStatus();
    gls_ret_glGetGraphicsResetStatus_t *ret = (gls_ret_glGetGraphicsResetStatus_t *)glsec_global.tmp_buf.buf;
    ret->cmd = GLSC_glGetGraphicsResetStatus;
    ret->status = status;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetGraphicsResetStatus_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glReadnPixels()
{
    GLSE_SET_COMMAND_PTR(c, glReadnPixels);
    glReadnPixels(c->x, c->y, c->width, c->height, c->format, c->type, c->bufSize, glsec_global.tmp_buf.buf);
    glse_cmd_send_data(0, c->bufSize, (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetnUniformfv()
{
    GLSE_SET_COMMAND_PTR(c, glGetnUniformfv);
    glGetnUniformfv(c->program, c->location, c->bufSize, (GLfloat *)glsec_global.tmp_buf.buf);
    glse_cmd_send_data(0, c->bufSize, (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetnUniformiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetnUniformiv);
    glGetnUniformiv(c->program, c->location, c->bufSize, (GLint *)glsec_global.tmp_buf.buf);
    glse_cmd_send_data(0, c->bufSize, (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetnUniformuiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetnUniformuiv);
    glGetnUniformuiv(c->program, c->location, c->bufSize, (GLuint *)glsec_global.tmp_buf.buf);
    glse_cmd_send_data(0, c->bufSize, (char *)glsec_global.tmp_buf.buf);
}

void glse_glMinSampleShading()
{
    GLSE_SET_COMMAND_PTR(c, glMinSampleShading);
    glMinSampleShading(c->value);
}

void glse_glPatchParameteri()
{
    GLSE_SET_COMMAND_PTR(c, glPatchParameteri);
    glPatchParameteri(c->pname, c->value);
}

void glse_glTexParameterIiv()
{
    GLSE_SET_COMMAND_PTR(c, glTexParameterIiv);
    glTexParameterIiv(c->target, c->pname, (const GLint *)glsec_global.tmp_buf.buf);
}

void glse_glTexParameterIuiv()
{
    GLSE_SET_COMMAND_PTR(c, glTexParameterIuiv);
    glTexParameterIuiv(c->target, c->pname, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glGetTexParameterIiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetTexParameterIiv);
    gls_ret_glGetTexParameterIiv_t *ret = (gls_ret_glGetTexParameterIiv_t *)glsec_global.tmp_buf.buf;
    glGetTexParameterIiv(c->target, c->pname, ret->params);
    ret->cmd = GLSC_glGetTexParameterIiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetTexParameterIiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetTexParameterIuiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetTexParameterIuiv);
    gls_ret_glGetTexParameterIuiv_t *ret = (gls_ret_glGetTexParameterIuiv_t *)glsec_global.tmp_buf.buf;
    glGetTexParameterIuiv(c->target, c->pname, (GLuint *)ret->params);
    ret->cmd = GLSC_glGetTexParameterIuiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetTexParameterIuiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glSamplerParameterIiv()
{
    GLSE_SET_COMMAND_PTR(c, glSamplerParameterIiv);
    glSamplerParameterIiv(c->sampler, c->pname, (const GLint *)glsec_global.tmp_buf.buf);
}

void glse_glSamplerParameterIuiv()
{
    GLSE_SET_COMMAND_PTR(c, glSamplerParameterIuiv);
    glSamplerParameterIuiv(c->sampler, c->pname, (const GLuint *)glsec_global.tmp_buf.buf);
}

void glse_glGetSamplerParameterIiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetSamplerParameterIiv);
    gls_ret_glGetSamplerParameterIiv_t *ret = (gls_ret_glGetSamplerParameterIiv_t *)glsec_global.tmp_buf.buf;
    glGetSamplerParameterIiv(c->sampler, c->pname, ret->params);
    ret->cmd = GLSC_glGetSamplerParameterIiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetSamplerParameterIiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glGetSamplerParameterIuiv()
{
    GLSE_SET_COMMAND_PTR(c, glGetSamplerParameterIuiv);
    gls_ret_glGetSamplerParameterIuiv_t *ret = (gls_ret_glGetSamplerParameterIuiv_t *)glsec_global.tmp_buf.buf;
    glGetSamplerParameterIuiv(c->sampler, c->pname, (GLuint *)ret->params);
    ret->cmd = GLSC_glGetSamplerParameterIuiv;
    glse_cmd_send_data(0, sizeof(gls_ret_glGetSamplerParameterIuiv_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_glTexBuffer()
{
    GLSE_SET_COMMAND_PTR(c, glTexBuffer);
    glTexBuffer(c->target, c->internalformat, c->buffer);
}

void glse_glTexBufferRange()
{
    GLSE_SET_COMMAND_PTR(c, glTexBufferRange);
    glTexBufferRange(c->target, c->internalformat, c->buffer, c->offset, c->size);
}

void glse_glTexStorage3DMultisample()
{
    GLSE_SET_COMMAND_PTR(c, glTexStorage3DMultisample);
    glTexStorage3DMultisample(c->target, c->samples, c->internalformat, c->width, c->height, c->depth, c->fixedsamplelocations);
}

int gles3_flushCommand(gls_command_t *c)
{
    switch (c->cmd) {
        case GLSC_glReadBuffer:
            pop_batch_command(sizeof(gls_glReadBuffer_t));
            break;
        case GLSC_glDrawRangeElements:
            pop_batch_command(((gls_glDrawRangeElements_t *)c)->cmd_size);
            break;
        case GLSC_glTexImage3D:
            pop_batch_command(((gls_glTexImage3D_t *)c)->cmd_size);
            break;
        case GLSC_glTexSubImage3D:
            pop_batch_command(((gls_glTexSubImage3D_t *)c)->cmd_size);
            break;
        case GLSC_glCopyTexSubImage3D:
            pop_batch_command(sizeof(gls_glCopyTexSubImage3D_t));
            break;
        case GLSC_glCompressedTexImage3D:
            pop_batch_command(((gls_glCompressedTexImage3D_t *)c)->cmd_size);
            break;
        case GLSC_glCompressedTexSubImage3D:
            pop_batch_command(((gls_glCompressedTexSubImage3D_t *)c)->cmd_size);
            break;
        case GLSC_glBeginQuery:
            pop_batch_command(sizeof(gls_glBeginQuery_t));
            break;
        case GLSC_glEndQuery:
            pop_batch_command(sizeof(gls_glEndQuery_t));
            break;
        case GLSC_glUniformMatrix2x3fv:
            pop_batch_command(sizeof(gls_glUniformMatrix2x3fv_t));
            break;
        case GLSC_glUniformMatrix3x2fv:
            pop_batch_command(sizeof(gls_glUniformMatrix3x2fv_t));
            break;
        case GLSC_glUniformMatrix2x4fv:
            pop_batch_command(sizeof(gls_glUniformMatrix2x4fv_t));
            break;
        case GLSC_glUniformMatrix4x2fv:
            pop_batch_command(sizeof(gls_glUniformMatrix4x2fv_t));
            break;
        case GLSC_glUniformMatrix3x4fv:
            pop_batch_command(sizeof(gls_glUniformMatrix3x4fv_t));
            break;
        case GLSC_glUniformMatrix4x3fv:
            pop_batch_command(sizeof(gls_glUniformMatrix4x3fv_t));
            break;
        case GLSC_glBlitFramebuffer:
            pop_batch_command(sizeof(gls_glBlitFramebuffer_t));
            break;
        case GLSC_glRenderbufferStorageMultisample:
            pop_batch_command(sizeof(gls_glRenderbufferStorageMultisample_t));
            break;
        case GLSC_glFramebufferTextureLayer:
            pop_batch_command(sizeof(gls_glFramebufferTextureLayer_t));
            break;
        case GLSC_glFlushMappedBufferRange:
            pop_batch_command(sizeof(gls_glFlushMappedBufferRange_t));
            break;
        case GLSC_glBindVertexArray:
            pop_batch_command(sizeof(gls_glBindVertexArray_t));
            break;
        case GLSC_glBeginTransformFeedback:
            pop_batch_command(sizeof(gls_glBeginTransformFeedback_t));
            break;
        case GLSC_glEndTransformFeedback:
            pop_batch_command(sizeof(gls_glEndTransformFeedback_t));
            break;
        case GLSC_glBindBufferRange:
            pop_batch_command(sizeof(gls_glBindBufferRange_t));
            break;
        case GLSC_glBindBufferBase:
            pop_batch_command(sizeof(gls_glBindBufferBase_t));
            break;
        case GLSC_glVertexAttribI4i:
            pop_batch_command(sizeof(gls_glVertexAttribI4i_t));
            break;
        case GLSC_glVertexAttribI4ui:
            pop_batch_command(sizeof(gls_glVertexAttribI4ui_t));
            break;
        case GLSC_glVertexAttribI4iv:
            pop_batch_command(sizeof(gls_glVertexAttribI4iv_t));
            break;
        case GLSC_glVertexAttribI4uiv:
            pop_batch_command(sizeof(gls_glVertexAttribI4uiv_t));
            break;
        case GLSC_glUniform1ui:
            pop_batch_command(sizeof(gls_glUniform1ui_t));
            break;
        case GLSC_glUniform2ui:
            pop_batch_command(sizeof(gls_glUniform2ui_t));
            break;
        case GLSC_glUniform3ui:
            pop_batch_command(sizeof(gls_glUniform3ui_t));
            break;
        case GLSC_glUniform4ui:
            pop_batch_command(sizeof(gls_glUniform4ui_t));
            break;
        case GLSC_glClearBufferfi:
            pop_batch_command(sizeof(gls_glClearBufferfi_t));
            break;
        case GLSC_glCopyBufferSubData:
            pop_batch_command(sizeof(gls_glCopyBufferSubData_t));
            break;
        case GLSC_glUniformBlockBinding:
            pop_batch_command(sizeof(gls_glUniformBlockBinding_t));
            break;
        case GLSC_glDrawArraysInstanced:
            pop_batch_command(sizeof(gls_glDrawArraysInstanced_t));
            break;
        case GLSC_glDrawElementsInstanced:
            pop_batch_command(sizeof(gls_glDrawElementsInstanced_t));
            break;
        case GLSC_glVertexAttribDivisor:
            pop_batch_command(sizeof(gls_glVertexAttribDivisor_t));
            break;
        case GLSC_glBindTransformFeedback:
            pop_batch_command(sizeof(gls_glBindTransformFeedback_t));
            break;
        case GLSC_glPauseTransformFeedback:
            pop_batch_command(sizeof(gls_glPauseTransformFeedback_t));
            break;
        case GLSC_glResumeTransformFeedback:
            pop_batch_command(sizeof(gls_glResumeTransformFeedback_t));
            break;
        case GLSC_glProgramParameteri:
            pop_batch_command(sizeof(gls_glProgramParameteri_t));
            break;
        case GLSC_glTexStorage2D:
            pop_batch_command(sizeof(gls_glTexStorage2D_t));
            break;
        case GLSC_glTexStorage3D:
            pop_batch_command(sizeof(gls_glTexStorage3D_t));
            break;
        case GLSC_glDispatchCompute:
            pop_batch_command(sizeof(gls_glDispatchCompute_t));
            break;
        case GLSC_glDispatchComputeIndirect:
            pop_batch_command(sizeof(gls_glDispatchComputeIndirect_t));
            break;
        case GLSC_glDrawArraysIndirect:
            pop_batch_command(sizeof(gls_glDrawArraysIndirect_t));
            break;
        case GLSC_glDrawElementsIndirect:
            pop_batch_command(sizeof(gls_glDrawElementsIndirect_t));
            break;
        case GLSC_glFramebufferParameteri:
            pop_batch_command(sizeof(gls_glFramebufferParameteri_t));
            break;
        case GLSC_glUseProgramStages:
            pop_batch_command(sizeof(gls_glUseProgramStages_t));
            break;
        case GLSC_glActiveShaderProgram:
            pop_batch_command(sizeof(gls_glActiveShaderProgram_t));
            break;
        case GLSC_glBindProgramPipeline:
            pop_batch_command(sizeof(gls_glBindProgramPipeline_t));
            break;
        case GLSC_glProgramUniform1i:
            pop_batch_command(sizeof(gls_glProgramUniform1i_t));
            break;
        case GLSC_glProgramUniform2i:
            pop_batch_command(sizeof(gls_glProgramUniform2i_t));
            break;
        case GLSC_glProgramUniform3i:
            pop_batch_command(sizeof(gls_glProgramUniform3i_t));
            break;
        case GLSC_glProgramUniform4i:
            pop_batch_command(sizeof(gls_glProgramUniform4i_t));
            break;
        case GLSC_glProgramUniform1ui:
            pop_batch_command(sizeof(gls_glProgramUniform1ui_t));
            break;
        case GLSC_glProgramUniform2ui:
            pop_batch_command(sizeof(gls_glProgramUniform2ui_t));
            break;
        case GLSC_glProgramUniform3ui:
            pop_batch_command(sizeof(gls_glProgramUniform3ui_t));
            break;
        case GLSC_glProgramUniform4ui:
            pop_batch_command(sizeof(gls_glProgramUniform4ui_t));
            break;
        case GLSC_glProgramUniform1f:
            pop_batch_command(sizeof(gls_glProgramUniform1f_t));
            break;
        case GLSC_glProgramUniform2f:
            pop_batch_command(sizeof(gls_glProgramUniform2f_t));
            break;
        case GLSC_glProgramUniform3f:
            pop_batch_command(sizeof(gls_glProgramUniform3f_t));
            break;
        case GLSC_glProgramUniform4f:
            pop_batch_command(sizeof(gls_glProgramUniform4f_t));
            break;
        case GLSC_glValidateProgramPipeline:
            pop_batch_command(sizeof(gls_glValidateProgramPipeline_t));
            break;
        case GLSC_glBindImageTexture:
            pop_batch_command(sizeof(gls_glBindImageTexture_t));
            break;
        case GLSC_glMemoryBarrier:
            pop_batch_command(sizeof(gls_glMemoryBarrier_t));
            break;
        case GLSC_glMemoryBarrierByRegion:
            pop_batch_command(sizeof(gls_glMemoryBarrierByRegion_t));
            break;
        case GLSC_glTexStorage2DMultisample:
            pop_batch_command(sizeof(gls_glTexStorage2DMultisample_t));
            break;
        case GLSC_glSampleMaski:
            pop_batch_command(sizeof(gls_glSampleMaski_t));
            break;
        case GLSC_glBindVertexBuffer:
            pop_batch_command(sizeof(gls_glBindVertexBuffer_t));
            break;
        case GLSC_glVertexAttribFormat:
            pop_batch_command(sizeof(gls_glVertexAttribFormat_t));
            break;
        case GLSC_glVertexAttribIFormat:
            pop_batch_command(sizeof(gls_glVertexAttribIFormat_t));
            break;
        case GLSC_glVertexAttribBinding:
            pop_batch_command(sizeof(gls_glVertexAttribBinding_t));
            break;
        case GLSC_glVertexBindingDivisor:
            pop_batch_command(sizeof(gls_glVertexBindingDivisor_t));
            break;
        case GLSC_glBlendBarrier:
            pop_batch_command(sizeof(gls_glBlendBarrier_t));
            break;
        case GLSC_glCopyImageSubData:
            pop_batch_command(sizeof(gls_glCopyImageSubData_t));
            break;
        case GLSC_glPopDebugGroup:
            pop_batch_command(sizeof(gls_glPopDebugGroup_t));
            break;
        case GLSC_glEnablei:
            pop_batch_command(sizeof(gls_glEnablei_t));
            break;
        case GLSC_glDisablei:
            pop_batch_command(sizeof(gls_glDisablei_t));
            break;
        case GLSC_glBlendEquationi:
            pop_batch_command(sizeof(gls_glBlendEquationi_t));
            break;
        case GLSC_glBlendEquationSeparatei:
            pop_batch_command(sizeof(gls_glBlendEquationSeparatei_t));
            break;
        case GLSC_glBlendFunci:
            pop_batch_command(sizeof(gls_glBlendFunci_t));
            break;
        case GLSC_glBlendFuncSeparatei:
            pop_batch_command(sizeof(gls_glBlendFuncSeparatei_t));
            break;
        case GLSC_glColorMaski:
            pop_batch_command(sizeof(gls_glColorMaski_t));
            break;
        case GLSC_glFramebufferTexture:
            pop_batch_command(sizeof(gls_glFramebufferTexture_t));
            break;
        case GLSC_glPrimitiveBoundingBox:
            pop_batch_command(sizeof(gls_glPrimitiveBoundingBox_t));
            break;
        case GLSC_glMinSampleShading:
            pop_batch_command(sizeof(gls_glMinSampleShading_t));
            break;
        case GLSC_glPatchParameteri:
            pop_batch_command(sizeof(gls_glPatchParameteri_t));
            break;
        case GLSC_glTexBuffer:
            pop_batch_command(sizeof(gls_glTexBuffer_t));
            break;
        case GLSC_glTexBufferRange:
            pop_batch_command(sizeof(gls_glTexBufferRange_t));
            break;
        case GLSC_glTexStorage3DMultisample:
            pop_batch_command(sizeof(gls_glTexStorage3DMultisample_t));
            break;
        default:
            return 0;
    }
    return 1;
}

int gles3_executeCommand(gls_command_t *c)
{
    switch (c->cmd) {
        case GLSC_glReadBuffer:
            glse_glReadBuffer();
            break;
        case GLSC_glDrawRangeElements:
            glse_glDrawRangeElements();
            break;
        case GLSC_glTexImage3D:
            glse_glTexImage3D();
            break;
        case GLSC_glTexSubImage3D:
            glse_glTexSubImage3D();
            break;
        case GLSC_glCopyTexSubImage3D:
            glse_glCopyTexSubImage3D();
            break;
        case GLSC_glCompressedTexImage3D:
            glse_glCompressedTexImage3D();
            break;
        case GLSC_glCompressedTexSubImage3D:
            glse_glCompressedTexSubImage3D();
            break;
        case GLSC_glGenQueries:
            glse_glGenQueries();
            break;
        case GLSC_glDeleteQueries:
            glse_glDeleteQueries();
            break;
        case GLSC_glIsQuery:
            glse_glIsQuery();
            break;
        case GLSC_glBeginQuery:
            glse_glBeginQuery();
            break;
        case GLSC_glEndQuery:
            glse_glEndQuery();
            break;
        case GLSC_glGetQueryiv:
            glse_glGetQueryiv();
            break;
        case GLSC_glGetQueryObjectuiv:
            glse_glGetQueryObjectuiv();
            break;
        case GLSC_glUnmapBuffer:
            glse_glUnmapBuffer();
            break;
        case GLSC_glGetBufferPointerv:
            glse_glGetBufferPointerv();
            break;
        case GLSC_glDrawBuffers:
            glse_glDrawBuffers();
            break;
        case GLSC_glUniformMatrix2x3fv:
            glse_glUniformMatrix2x3fv();
            break;
        case GLSC_glUniformMatrix3x2fv:
            glse_glUniformMatrix3x2fv();
            break;
        case GLSC_glUniformMatrix2x4fv:
            glse_glUniformMatrix2x4fv();
            break;
        case GLSC_glUniformMatrix4x2fv:
            glse_glUniformMatrix4x2fv();
            break;
        case GLSC_glUniformMatrix3x4fv:
            glse_glUniformMatrix3x4fv();
            break;
        case GLSC_glUniformMatrix4x3fv:
            glse_glUniformMatrix4x3fv();
            break;
        case GLSC_glBlitFramebuffer:
            glse_glBlitFramebuffer();
            break;
        case GLSC_glRenderbufferStorageMultisample:
            glse_glRenderbufferStorageMultisample();
            break;
        case GLSC_glFramebufferTextureLayer:
            glse_glFramebufferTextureLayer();
            break;
        case GLSC_glMapBufferRange:
            glse_glMapBufferRange();
            break;
        case GLSC_glFlushMappedBufferRange:
            glse_glFlushMappedBufferRange();
            break;
        case GLSC_glBindVertexArray:
            glse_glBindVertexArray();
            break;
        case GLSC_glDeleteVertexArrays:
            glse_glDeleteVertexArrays();
            break;
        case GLSC_glGenVertexArrays:
            glse_glGenVertexArrays();
            break;
        case GLSC_glIsVertexArray:
            glse_glIsVertexArray();
            break;
        case GLSC_glGetIntegeri_v:
            glse_glGetIntegeri_v();
            break;
        case GLSC_glBeginTransformFeedback:
            glse_glBeginTransformFeedback();
            break;
        case GLSC_glEndTransformFeedback:
            glse_glEndTransformFeedback();
            break;
        case GLSC_glBindBufferRange:
            glse_glBindBufferRange();
            break;
        case GLSC_glBindBufferBase:
            glse_glBindBufferBase();
            break;
        case GLSC_glTransformFeedbackVaryings:
            glse_glTransformFeedbackVaryings();
            break;
        case GLSC_glGetTransformFeedbackVarying:
            glse_glGetTransformFeedbackVarying();
            break;
        case GLSC_glVertexAttribIPointer:
            glse_glVertexAttribIPointer();
            break;
        case GLSC_glGetVertexAttribIiv:
            glse_glGetVertexAttribIiv();
            break;
        case GLSC_glGetVertexAttribIuiv:
            glse_glGetVertexAttribIuiv();
            break;
        case GLSC_glVertexAttribI4i:
            glse_glVertexAttribI4i();
            break;
        case GLSC_glVertexAttribI4ui:
            glse_glVertexAttribI4ui();
            break;
        case GLSC_glVertexAttribI4iv:
            glse_glVertexAttribI4iv();
            break;
        case GLSC_glVertexAttribI4uiv:
            glse_glVertexAttribI4uiv();
            break;
        case GLSC_glGetUniformuiv:
            glse_glGetUniformuiv();
            break;
        case GLSC_glGetFragDataLocation:
            glse_glGetFragDataLocation();
            break;
        case GLSC_glUniform1ui:
            glse_glUniform1ui();
            break;
        case GLSC_glUniform2ui:
            glse_glUniform2ui();
            break;
        case GLSC_glUniform3ui:
            glse_glUniform3ui();
            break;
        case GLSC_glUniform4ui:
            glse_glUniform4ui();
            break;
        case GLSC_glUniform1uiv:
            glse_glUniform1uiv();
            break;
        case GLSC_glUniform2uiv:
            glse_glUniform2uiv();
            break;
        case GLSC_glUniform3uiv:
            glse_glUniform3uiv();
            break;
        case GLSC_glUniform4uiv:
            glse_glUniform4uiv();
            break;
        case GLSC_glClearBufferiv:
            glse_glClearBufferiv();
            break;
        case GLSC_glClearBufferuiv:
            glse_glClearBufferuiv();
            break;
        case GLSC_glClearBufferfv:
            glse_glClearBufferfv();
            break;
        case GLSC_glClearBufferfi:
            glse_glClearBufferfi();
            break;
        case GLSC_glGetStringi:
            glse_glGetStringi();
            break;
        case GLSC_glCopyBufferSubData:
            glse_glCopyBufferSubData();
            break;
        case GLSC_glGetUniformIndices:
            glse_glGetUniformIndices();
            break;
        case GLSC_glGetActiveUniformsiv:
            glse_glGetActiveUniformsiv();
            break;
        case GLSC_glGetUniformBlockIndex:
            glse_glGetUniformBlockIndex();
            break;
        case GLSC_glGetActiveUniformBlockiv:
            glse_glGetActiveUniformBlockiv();
            break;
        case GLSC_glGetActiveUniformBlockName:
            glse_glGetActiveUniformBlockName();
            break;
        case GLSC_glUniformBlockBinding:
            glse_glUniformBlockBinding();
            break;
        case GLSC_glDrawArraysInstanced:
            glse_glDrawArraysInstanced();
            break;
        case GLSC_glDrawElementsInstanced:
            glse_glDrawElementsInstanced();
            break;
        case GLSC_glFenceSync:
            glse_glFenceSync();
            break;
        case GLSC_glIsSync:
            glse_glIsSync();
            break;
        case GLSC_glDeleteSync:
            glse_glDeleteSync();
            break;
        case GLSC_glClientWaitSync:
            glse_glClientWaitSync();
            break;
        case GLSC_glWaitSync:
            glse_glWaitSync();
            break;
        case GLSC_glGetInteger64v:
            glse_glGetInteger64v();
            break;
        case GLSC_glGetSynciv:
            glse_glGetSynciv();
            break;
        case GLSC_glGetInteger64i_v:
            glse_glGetInteger64i_v();
            break;
        case GLSC_glGetBufferParameteri64v:
            glse_glGetBufferParameteri64v();
            break;
        case GLSC_glGenSamplers:
            glse_glGenSamplers();
            break;
        case GLSC_glDeleteSamplers:
            glse_glDeleteSamplers();
            break;
        case GLSC_glIsSampler:
            glse_glIsSampler();
            break;
        case GLSC_glBindSampler:
            glse_glBindSampler();
            break;
        case GLSC_glSamplerParameteri:
            glse_glSamplerParameteri();
            break;
        case GLSC_glSamplerParameteriv:
            glse_glSamplerParameteriv();
            break;
        case GLSC_glSamplerParameterf:
            glse_glSamplerParameterf();
            break;
        case GLSC_glSamplerParameterfv:
            glse_glSamplerParameterfv();
            break;
        case GLSC_glGetSamplerParameteriv:
            glse_glGetSamplerParameteriv();
            break;
        case GLSC_glGetSamplerParameterfv:
            glse_glGetSamplerParameterfv();
            break;
        case GLSC_glVertexAttribDivisor:
            glse_glVertexAttribDivisor();
            break;
        case GLSC_glBindTransformFeedback:
            glse_glBindTransformFeedback();
            break;
        case GLSC_glDeleteTransformFeedbacks:
            glse_glDeleteTransformFeedbacks();
            break;
        case GLSC_glGenTransformFeedbacks:
            glse_glGenTransformFeedbacks();
            break;
        case GLSC_glIsTransformFeedback:
            glse_glIsTransformFeedback();
            break;
        case GLSC_glPauseTransformFeedback:
            glse_glPauseTransformFeedback();
            break;
        case GLSC_glResumeTransformFeedback:
            glse_glResumeTransformFeedback();
            break;
        case GLSC_glGetProgramBinary:
            glse_glGetProgramBinary();
            break;
        case GLSC_glProgramBinary:
            glse_glProgramBinary();
            break;
        case GLSC_glProgramParameteri:
            glse_glProgramParameteri();
            break;
        case GLSC_glInvalidateFramebuffer:
            glse_glInvalidateFramebuffer();
            break;
        case GLSC_glInvalidateSubFramebuffer:
            glse_glInvalidateSubFramebuffer();
            break;
        case GLSC_glTexStorage2D:
            glse_glTexStorage2D();
            break;
        case GLSC_glTexStorage3D:
            glse_glTexStorage3D();
            break;
        case GLSC_glGetInternalformativ:
            glse_glGetInternalformativ();
            break;
        case GLSC_glDispatchCompute:
            glse_glDispatchCompute();
            break;
        case GLSC_glDispatchComputeIndirect:
            glse_glDispatchComputeIndirect();
            break;
        case GLSC_glDrawArraysIndirect:
            glse_glDrawArraysIndirect();
            break;
        case GLSC_glDrawElementsIndirect:
            glse_glDrawElementsIndirect();
            break;
        case GLSC_glFramebufferParameteri:
            glse_glFramebufferParameteri();
            break;
        case GLSC_glGetFramebufferParameteriv:
            glse_glGetFramebufferParameteriv();
            break;
        case GLSC_glGetProgramInterfaceiv:
            glse_glGetProgramInterfaceiv();
            break;
        case GLSC_glGetProgramResourceIndex:
            glse_glGetProgramResourceIndex();
            break;
        case GLSC_glGetProgramResourceName:
            glse_glGetProgramResourceName();
            break;
        case GLSC_glGetProgramResourceiv:
            glse_glGetProgramResourceiv();
            break;
        case GLSC_glGetProgramResourceLocation:
            glse_glGetProgramResourceLocation();
            break;
        case GLSC_glUseProgramStages:
            glse_glUseProgramStages();
            break;
        case GLSC_glActiveShaderProgram:
            glse_glActiveShaderProgram();
            break;
        case GLSC_glCreateShaderProgramv:
            glse_glCreateShaderProgramv();
            break;
        case GLSC_glBindProgramPipeline:
            glse_glBindProgramPipeline();
            break;
        case GLSC_glDeleteProgramPipelines:
            glse_glDeleteProgramPipelines();
            break;
        case GLSC_glGenProgramPipelines:
            glse_glGenProgramPipelines();
            break;
        case GLSC_glIsProgramPipeline:
            glse_glIsProgramPipeline();
            break;
        case GLSC_glGetProgramPipelineiv:
            glse_glGetProgramPipelineiv();
            break;
        case GLSC_glProgramUniform1i:
            glse_glProgramUniform1i();
            break;
        case GLSC_glProgramUniform2i:
            glse_glProgramUniform2i();
            break;
        case GLSC_glProgramUniform3i:
            glse_glProgramUniform3i();
            break;
        case GLSC_glProgramUniform4i:
            glse_glProgramUniform4i();
            break;
        case GLSC_glProgramUniform1ui:
            glse_glProgramUniform1ui();
            break;
        case GLSC_glProgramUniform2ui:
            glse_glProgramUniform2ui();
            break;
        case GLSC_glProgramUniform3ui:
            glse_glProgramUniform3ui();
            break;
        case GLSC_glProgramUniform4ui:
            glse_glProgramUniform4ui();
            break;
        case GLSC_glProgramUniform1f:
            glse_glProgramUniform1f();
            break;
        case GLSC_glProgramUniform2f:
            glse_glProgramUniform2f();
            break;
        case GLSC_glProgramUniform3f:
            glse_glProgramUniform3f();
            break;
        case GLSC_glProgramUniform4f:
            glse_glProgramUniform4f();
            break;
        case GLSC_glProgramUniform1iv:
            glse_glProgramUniform1iv();
            break;
        case GLSC_glProgramUniform2iv:
            glse_glProgramUniform2iv();
            break;
        case GLSC_glProgramUniform3iv:
            glse_glProgramUniform3iv();
            break;
        case GLSC_glProgramUniform4iv:
            glse_glProgramUniform4iv();
            break;
        case GLSC_glProgramUniform1uiv:
            glse_glProgramUniform1uiv();
            break;
        case GLSC_glProgramUniform2uiv:
            glse_glProgramUniform2uiv();
            break;
        case GLSC_glProgramUniform3uiv:
            glse_glProgramUniform3uiv();
            break;
        case GLSC_glProgramUniform4uiv:
            glse_glProgramUniform4uiv();
            break;
        case GLSC_glProgramUniform1fv:
            glse_glProgramUniform1fv();
            break;
        case GLSC_glProgramUniform2fv:
            glse_glProgramUniform2fv();
            break;
        case GLSC_glProgramUniform3fv:
            glse_glProgramUniform3fv();
            break;
        case GLSC_glProgramUniform4fv:
            glse_glProgramUniform4fv();
            break;
        case GLSC_glProgramUniformMatrix2fv:
            glse_glProgramUniformMatrix2fv();
            break;
        case GLSC_glProgramUniformMatrix3fv:
            glse_glProgramUniformMatrix3fv();
            break;
        case GLSC_glProgramUniformMatrix4fv:
            glse_glProgramUniformMatrix4fv();
            break;
        case GLSC_glProgramUniformMatrix2x3fv:
            glse_glProgramUniformMatrix2x3fv();
            break;
        case GLSC_glProgramUniformMatrix3x2fv:
            glse_glProgramUniformMatrix3x2fv();
            break;
        case GLSC_glProgramUniformMatrix2x4fv:
            glse_glProgramUniformMatrix2x4fv();
            break;
        case GLSC_glProgramUniformMatrix4x2fv:
            glse_glProgramUniformMatrix4x2fv();
            break;
        case GLSC_glProgramUniformMatrix3x4fv:
            glse_glProgramUniformMatrix3x4fv();
            break;
        case GLSC_glProgramUniformMatrix4x3fv:
            glse_glProgramUniformMatrix4x3fv();
            break;
        case GLSC_glValidateProgramPipeline:
            glse_glValidateProgramPipeline();
            break;
        case GLSC_glGetProgramPipelineInfoLog:
            glse_glGetProgramPipelineInfoLog();
            break;
        case GLSC_glBindImageTexture:
            glse_glBindImageTexture();
            break;
        case GLSC_glGetBooleani_v:
            glse_glGetBooleani_v();
            break;
        case GLSC_glMemoryBarrier:
            glse_glMemoryBarrier();
            break;
        case GLSC_glMemoryBarrierByRegion:
            glse_glMemoryBarrierByRegion();
            break;
        case GLSC_glTexStorage2DMultisample:
            glse_glTexStorage2DMultisample();
            break;
        case GLSC_glGetMultisamplefv:
            glse_glGetMultisamplefv();
            break;
        case GLSC_glSampleMaski:
            glse_glSampleMaski();
            break;
        case GLSC_glGetTexLevelParameteriv:
            glse_glGetTexLevelParameteriv();
            break;
        case GLSC_glGetTexLevelParameterfv:
            glse_glGetTexLevelParameterfv();
            break;
        case GLSC_glBindVertexBuffer:
            glse_glBindVertexBuffer();
            break;
        case GLSC_glVertexAttribFormat:
            glse_glVertexAttribFormat();
            break;
        case GLSC_glVertexAttribIFormat:
            glse_glVertexAttribIFormat();
            break;
        case GLSC_glVertexAttribBinding:
            glse_glVertexAttribBinding();
            break;
        case GLSC_glVertexBindingDivisor:
            glse_glVertexBindingDivisor();
            break;
        case GLSC_glBlendBarrier:
            glse_glBlendBarrier();
            break;
        case GLSC_glCopyImageSubData:
            glse_glCopyImageSubData();
            break;
        case GLSC_glDebugMessageControl:
            glse_glDebugMessageControl();
            break;
        case GLSC_glDebugMessageInsert:
            glse_glDebugMessageInsert();
            break;
        case GLSC_glDebugMessageCallback:
            glse_glDebugMessageCallback();
            break;
        case GLSC_glGetDebugMessageLog:
            glse_glGetDebugMessageLog();
            break;
        case GLSC_glPushDebugGroup:
            glse_glPushDebugGroup();
            break;
        case GLSC_glPopDebugGroup:
            glse_glPopDebugGroup();
            break;
        case GLSC_glObjectLabel:
            glse_glObjectLabel();
            break;
        case GLSC_glGetObjectLabel:
            glse_glGetObjectLabel();
            break;
        case GLSC_glObjectPtrLabel:
            glse_glObjectPtrLabel();
            break;
        case GLSC_glGetObjectPtrLabel:
            glse_glGetObjectPtrLabel();
            break;
        case GLSC_glGetPointerv:
            glse_glGetPointerv();
            break;
        case GLSC_glEnablei:
            glse_glEnablei();
            break;
        case GLSC_glDisablei:
            glse_glDisablei();
            break;
        case GLSC_glBlendEquationi:
            glse_glBlendEquationi();
            break;
        case GLSC_glBlendEquationSeparatei:
            glse_glBlendEquationSeparatei();
            break;
        case GLSC_glBlendFunci:
            glse_glBlendFunci();
            break;
        case GLSC_glBlendFuncSeparatei:
            glse_glBlendFuncSeparatei();
            break;
        case GLSC_glColorMaski:
            glse_glColorMaski();
            break;
        case GLSC_glIsEnabledi:
            glse_glIsEnabledi();
            break;
        case GLSC_glDrawElementsBaseVertex:
            glse_glDrawElementsBaseVertex();
            break;
        case GLSC_glDrawRangeElementsBaseVertex:
            glse_glDrawRangeElementsBaseVertex();
            break;
        case GLSC_glDrawElementsInstancedBaseVertex:
            glse_glDrawElementsInstancedBaseVertex();
            break;
        case GLSC_glFramebufferTexture:
            glse_glFramebufferTexture();
            break;
        case GLSC_glPrimitiveBoundingBox:
            glse_glPrimitiveBoundingBox();
            break;
        case GLSC_glGetGraphicsResetStatus:
            glse_glGetGraphicsResetStatus();
            break;
        case GLSC_glReadnPixels:
            glse_glReadnPixels();
            break;
        case GLSC_glGetnUniformfv:
            glse_glGetnUniformfv();
            break;
        case GLSC_glGetnUniformiv:
            glse_glGetnUniformiv();
            break;
        case GLSC_glGetnUniformuiv:
            glse_glGetnUniformuiv();
            break;
        case GLSC_glMinSampleShading:
            glse_glMinSampleShading();
            break;
        case GLSC_glPatchParameteri:
            glse_glPatchParameteri();
            break;
        case GLSC_glTexParameterIiv:
            glse_glTexParameterIiv();
            break;
        case GLSC_glTexParameterIuiv:
            glse_glTexParameterIuiv();
            break;
        case GLSC_glGetTexParameterIiv:
            glse_glGetTexParameterIiv();
            break;
        case GLSC_glGetTexParameterIuiv:
            glse_glGetTexParameterIuiv();
            break;
        case GLSC_glSamplerParameterIiv:
            glse_glSamplerParameterIiv();
            break;
        case GLSC_glSamplerParameterIuiv:
            glse_glSamplerParameterIuiv();
            break;
        case GLSC_glGetSamplerParameterIiv:
            glse_glGetSamplerParameterIiv();
            break;
        case GLSC_glGetSamplerParameterIuiv:
            glse_glGetSamplerParameterIuiv();
            break;
        case GLSC_glTexBuffer:
            glse_glTexBuffer();
            break;
        case GLSC_glTexBufferRange:
            glse_glTexBufferRange();
            break;
        case GLSC_glTexStorage3DMultisample:
            glse_glTexStorage3DMultisample();
            break;
        default:
            return 0;
    }
    return 1;
}