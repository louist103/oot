#include "global.h"
#include "vt.h"

static Gfx sRCPSetupFade[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                          G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsDPSetOtherMode(G_AD_DISABLE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_1PRIMITIVE,
                     G_AC_NONE | G_ZS_PIXEL | G_RM_CLD_SURF | G_RM_CLD_SURF2),
    gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, PRIMITIVE, 0, 0, 0, PRIMITIVE, 0, 0, 0, PRIMITIVE),
    gsSPEndDisplayList(),
};

#define THIS ((TransitionFade*)thisx)

void TransitionFade_Start(void* thisx) {
    TransitionFade* self = THIS;

    switch (self->fadeType) {
        case 0:
            break;
        case 1:
            self->fadeTimer = 0;
            self->fadeColor.a = self->fadeDirection != 0 ? 0xFF : 0;
            break;
        case 2:
            self->fadeColor.a = 0;
            break;
    }
    self->isDone = 0;
}

void* TransitionFade_Init(void* thisx) {
    TransitionFade* self = THIS;

    bzero(self, sizeof(*self));
    return self;
}

void TransitionFade_Destroy(void* thisx) {
}

void TransitionFade_Update(void* thisx, s32 updateRate) {
    s32 alpha;
    s16 newAlpha;
    TransitionFade* self = THIS;

    switch (self->fadeType) {
        case 0:
            break;
        case 1:
            self->fadeTimer += updateRate;
            if (self->fadeTimer >= gSaveContext.fadeDuration) {
                self->fadeTimer = gSaveContext.fadeDuration;
                self->isDone = 1;
            }
            if (!gSaveContext.fadeDuration) {
                // "Divide by 0! Zero is included in ZCommonGet fade_speed"
                osSyncPrintf(VT_COL(RED, WHITE) "０除算! ZCommonGet fade_speed に０がはいってる" VT_RST);
            }

            alpha = (255.0f * self->fadeTimer) / ((void)0, gSaveContext.fadeDuration);
            self->fadeColor.a = (self->fadeDirection != 0) ? 255 - alpha : alpha;
            break;
        case 2:
            newAlpha = self->fadeColor.a;
            if (iREG(50) != 0) {
                if (iREG(50) < 0) {
                    if (Math_StepToS(&newAlpha, 255, 255)) {
                        iREG(50) = 150;
                    }
                } else {
                    Math_StepToS(&iREG(50), 20, 60);
                    if (Math_StepToS(&newAlpha, 0, iREG(50))) {
                        iREG(50) = 0;
                        self->isDone = 1;
                    }
                }
            }
            self->fadeColor.a = newAlpha;
            break;
    }
}

void TransitionFade_Draw(void* thisx, Gfx** gfxP) {
    TransitionFade* self = THIS;
    Gfx* gfx;
    Color_RGBA8_u32* color = &self->fadeColor;

    if (color->a > 0) {
        gfx = *gfxP;
        gSPDisplayList(gfx++, sRCPSetupFade);
        gDPSetPrimColor(gfx++, 0, 0, color->r, color->g, color->b, color->a);
        gDPFillRectangle(gfx++, 0, 0, gScreenWidth - 1, gScreenHeight - 1);
        gDPPipeSync(gfx++);
        *gfxP = gfx;
    }
}

s32 TransitionFade_IsDone(void* thisx) {
    TransitionFade* self = THIS;

    return self->isDone;
}

void TransitionFade_SetColor(void* thisx, u32 color) {
    TransitionFade* self = THIS;

    self->fadeColor.rgba = color;
}

void TransitionFade_SetType(void* thisx, s32 type) {
    TransitionFade* self = THIS;

    if (type == 1) {
        self->fadeType = 1;
        self->fadeDirection = 1;
    } else if (type == 2) {
        self->fadeType = 1;
        self->fadeDirection = 0;
    } else if (type == 3) {
        self->fadeType = 2;
    } else {
        self->fadeType = 0;
    }
}
