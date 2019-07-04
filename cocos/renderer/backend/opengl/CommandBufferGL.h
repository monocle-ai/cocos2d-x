/****************************************************************************
 Copyright (c) 2018-2019 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
 
#pragma once

#include "../Macros.h"
#include "../CommandBuffer.h"
#include "base/CCEventListenerCustom.h"
#include "platform/CCGL.h"

#include "CCStdC.h"

#include <vector>

CC_BACKEND_BEGIN

class BufferGL;
class RenderPipelineGL;
class ProgramGL;
class DepthStencilStateGL;

class CommandBufferGL final : public CommandBuffer
{
public:
    CommandBufferGL();
    ~CommandBufferGL();
    
    virtual void beginFrame() override;
    virtual void beginRenderPass(const RenderPassDescriptor& descriptor) override;
    virtual void setRenderPipeline(RenderPipeline* renderPipeline) override;
    virtual void setViewport(int x, int y, unsigned int w, unsigned int h) override;
    virtual void setCullMode(CullMode mode) override;
    virtual void setWinding(Winding winding) override;
    virtual void setVertexBuffer(unsigned int index, Buffer* buffer) override;
    virtual void setProgramState(ProgramState* programState) override;
    virtual void setIndexBuffer(Buffer* buffer) override;
    virtual void drawArrays(PrimitiveType primitiveType, unsigned int start,  unsigned int count) override;
    virtual void drawElements(PrimitiveType primitiveType, IndexFormat indexType, unsigned int count, unsigned int offset) override;
    virtual void endRenderPass() override;
    virtual void endFrame() override;
    
    virtual void setLineWidth(float lineWidth) override;
    
    virtual void setScissorRect(bool isEnabled, float x, float y, float width, float height) override;
    virtual void setDepthStencilState(DepthStencilState* depthStencilState) override;
    virtual void captureScreen(std::function<void(const unsigned char*, int, int)> callback) override ;

private:
    struct Viewport
    {
        int x = 0;
        int y = 0;
        unsigned int w = 0;
        unsigned int h = 0;
    };
    
    void prepareDrawing() const;
    void bindVertexBuffer(ProgramGL* program) const;
    void setUniforms(ProgramGL* program) const;
    void setUniform(bool isArray, GLuint location, unsigned int size, GLenum uniformType, void* data) const;
    void cleanResources();
    void applyRenderPassDescriptor(const RenderPassDescriptor& descirptor);

    // The frame buffer generated by engine. All frame buffer other than default frame buffer share it.
    GLuint _generatedFBO = 0;
    bool _generatedFBOBindColor = false;
    bool _generatedFBOBindDepth = false;
    bool _generatedFBOBindStencil = false;

    GLint _defaultFBO = 0;  // The value gets from glGetIntegerv, so need to use GLint
    GLuint _currentFBO = 0;
    std::vector<BufferGL*> _vertexBuffers;
    ProgramState* _programState = nullptr;
    BufferGL* _indexBuffer = nullptr;
    RenderPipelineGL* _renderPipeline = nullptr;
    CullMode _cullMode = CullMode::NONE;
    DepthStencilStateGL* _depthStencilStateGL = nullptr;
    Viewport _viewPort;
    GLboolean _alphaTestEnabled = false;

#if CC_ENABLE_CACHE_TEXTURE_DATA
    EventListenerCustom* _backToForegroundListener = nullptr;
#endif
};

CC_BACKEND_END
