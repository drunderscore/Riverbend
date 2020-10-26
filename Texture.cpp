#include "Texture.h"

Texture::Texture(const u8* data, u32 width, u32 height, Format fmt , Type t, u32 count) : m_width(width), m_height(height), m_number_of_textures(count)
{
    glGenTextures(count, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(fmt), width, height, 0, static_cast<GLenum>(fmt), static_cast<GLenum>(t), data);
}

Texture::~Texture()
{
    glDeleteTextures(m_number_of_textures, &m_id);
}
