#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

#include "../Pass.h"

class AverageLuminance : public Pass
{
public:

    AverageLuminance(const Pipeline * pipeline);
    ~AverageLuminance(void);

    bool	init	(const GPU::Texture<GL_TEXTURE_2D> * pTexture1, const GPU::Texture<GL_TEXTURE_2D> * pTexture2);
    void	free	(void);

    bool	begin	(void);
    bool	end		(void);

    unsigned int    next    (void);

    float getAverage(void);
    float getMax2(void);

private:

    unsigned int m_uCurrentObject;

    float m_fSumLog;
    float m_fMax;
};
