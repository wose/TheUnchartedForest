#include <iostream>
#include <png.h>

#include "texture.h"

CTexture::CTexture()
{
}

CTexture::CTexture(const std::string &strFileName)
{
  LoadPNG("resources/textures/" + strFileName + ".png");
}

CTexture::~CTexture()
{
}

////////////////////////////////////////////////////////////////////////////////
void CTexture::LoadPNG(const std::string &strFileName)
{
  png_structp pPNG = nullptr;
  png_infop pInfo = nullptr;
  png_bytep *pRows = nullptr;
  int nBitDepth, nColorType;

  FILE *pngFile = fopen(strFileName.c_str(), "rb");

  if(pngFile)
    {
      png_byte sig[8];

      fread(&sig, 8, sizeof(png_byte), pngFile);
      rewind(pngFile);
      if(!png_check_sig(sig, 8))
        {
          std::cerr << "png sig failure" << std::endl;
        }
      else
        {
          pPNG = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
          if(!pPNG)
            {
              std::cerr << "png ptr not created" << std::endl;
            }
          else
            {
              if(setjmp(png_jmpbuf(pPNG)))
                {
                  std::cerr << "set jmp failed" << std::endl;
                }
              else
                {
                  pInfo = png_create_info_struct(pPNG);
                  if(!pInfo)
                    {
                      std::cerr << "cant get png info ptr" << std::endl;
                    }
                  else
                    {
                      png_init_io(pPNG, pngFile);
                      png_read_info(pPNG, pInfo);
                      nBitDepth = png_get_bit_depth(pPNG, pInfo);
                      nColorType = png_get_color_type(pPNG, pInfo);

                      if(nColorType == PNG_COLOR_TYPE_PALETTE)
                        png_set_palette_to_rgb(pPNG);
                      if(nColorType == PNG_COLOR_TYPE_GRAY && nBitDepth < 8)
                        png_set_expand_gray_1_2_4_to_8(pPNG);
                      if(png_get_valid(pPNG, pInfo, PNG_INFO_tRNS))
                        png_set_tRNS_to_alpha(pPNG);

                      if(nBitDepth == 16)
                        png_set_strip_16(pPNG);
                      else if(nBitDepth < 8)
                        png_set_packing(pPNG);

                      png_read_update_info(pPNG, pInfo);

                      png_uint_32 nWidth, nHeight;
                      png_get_IHDR(pPNG, pInfo, &nWidth, &nHeight,
                                   &nBitDepth, &nColorType, NULL, NULL, NULL);

                      int nComponents; // = GetTextureInfo(colorType);
                      switch (nColorType)
                        {
                        case PNG_COLOR_TYPE_GRAY:
                          nComponents = 1;
                          break;
                        case PNG_COLOR_TYPE_GRAY_ALPHA:
                          nComponents = 2;
                          break;
                        case PNG_COLOR_TYPE_RGB:
                          nComponents = 3;
                          break;
                        case PNG_COLOR_TYPE_RGB_ALPHA:
                          nComponents = 4;
                          break;
                        default:
                          nComponents = -1;
                        }

                      if(nComponents == -1) {
                        if(pPNG)
                          png_destroy_read_struct(&pPNG, &pInfo, NULL);
                        std::cerr << strFileName << " broken?" << std::endl;
                      }
                      else
                        {
                          GLubyte *pPixels =
                            (GLubyte *) malloc(sizeof(GLubyte) * (nWidth * nHeight * nComponents));
                          pRows = (png_bytep *) malloc(sizeof(png_bytep) * nHeight);

                          for(int i = 0; i < nHeight; ++i)
                            pRows[i] = (png_bytep) (pPixels + (i * nWidth * nComponents));
                          
                          png_read_image(pPNG, pRows);
                          png_read_end(pPNG, NULL);

                          glGenTextures(1, &m_nTextureID);
                          glBindTexture(GL_TEXTURE_2D, m_nTextureID);
                          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                          GLuint glColors;
                          (nComponents == 4) ? (glColors = GL_RGBA) : (0);
                          (nComponents == 3) ? (glColors = GL_RGB) : (0);
                          // (nComponents == 2) ? (glColors = GL_LUMINANCE_ALPHA) : (0);
                          // (nComponents == 1) ? (glColors = GL_LUMINANCE) : (0);

  //printf("%s has %i colour components\n",filename,components);
  //glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, glcolours, GL_UNSIGNED_BYTE, pixels);
                          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA,
                                       GL_UNSIGNED_BYTE, pPixels);

                          png_destroy_read_struct(&pPNG, &pInfo, NULL);

                          fclose(pngFile);
                          free(pRows);
                          free(pPixels);
                        }
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
int CTexture::GetTextureID()
{
  return m_nTextureID;
}
