
#include "./OtherClasses/GLClasses.hpp"

#include <glad/glad.h>

namespace GLClasses {

    Framebuffer::Framebuffer() {
        glGenFramebuffers(1, &this->buffer);
    }
    Framebuffer::~Framebuffer() {
        glDeleteFramebuffers(1, &this->buffer);
    }

    void Framebuffer::BindBuffer() const {
        glBindFramebuffer(GL_FRAMEBUFFER, this->buffer);
    }
    void Framebuffer::BindDefaultFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::AttachTexture(GLuint renderTexture, GLenum attachmentType) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, renderTexture, 0);
    }

    void Framebuffer::AttachRenderTexture(GLuint renderTexture) {
        AttachTexture(renderTexture, GL_COLOR_ATTACHMENT0);
    }
    void Framebuffer::AttachDepthStencilTexture(GLuint renderTexture) {
        AttachTexture(renderTexture, GL_DEPTH_STENCIL_ATTACHMENT);
    }

    void Framebuffer::AttachDepthTexture(GLuint renderTexture) {
        AttachTexture(renderTexture, GL_DEPTH_ATTACHMENT);
    }

    void Framebuffer::AttachStencilTexture(GLuint renderTexture) {
        AttachTexture(renderTexture, GL_STENCIL_ATTACHMENT);
    }

    bool Framebuffer::CheckComplete() {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }
};