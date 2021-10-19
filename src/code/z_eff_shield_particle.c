#include "global.h"
#include "vt.h"
#include "objects/gameplay_keep/gameplay_keep.h"

static Vtx sVertices[5] = {
    VTX(-32, -32, 0, 0, 1024, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(32, 32, 0, 1024, 0, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(-32, 32, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(32, -32, 0, 1024, 1024, 0xFF, 0xFF, 0xFF, 0xFF),
};

// original name: "EffectShieldParticle_ct"
void EffectShieldParticle_Init(void* thisx, void* initParamsx) {
    EffectShieldParticle* self = (EffectShieldParticle*)thisx;
    EffectShieldParticleInit* initParams = (EffectShieldParticleInit*)initParamsx;
    EffectShieldParticleElement* elem;

    if ((self != NULL) && (initParams != NULL)) {
        self->numElements = initParams->numElements;
        if (self->numElements > ARRAY_COUNT(self->elements)) {
            osSyncPrintf(VT_FGCOL(RED));
            osSyncPrintf("EffectShieldParticle_ct():パーティクル数がオーバしてます。\n");
            osSyncPrintf(VT_RST);
            return;
        }

        self->position = initParams->position;
        self->primColorStart = initParams->primColorStart;
        self->envColorStart = initParams->envColorStart;
        self->primColorMid = initParams->primColorMid;
        self->envColorMid = initParams->envColorMid;
        self->primColorEnd = initParams->primColorEnd;
        self->envColorEnd = initParams->envColorEnd;
        self->deceleration = initParams->deceleration;
        self->maxInitialSpeed = initParams->maxInitialSpeed;
        self->lengthCutoff = initParams->lengthCutoff;
        self->duration = initParams->duration;
        self->timer = 0;

        for (elem = &self->elements[0]; elem < &self->elements[self->numElements]; elem++) {
            elem->initialSpeed = (Rand_ZeroOne() * (self->maxInitialSpeed * 0.5f)) + (self->maxInitialSpeed * 0.5f);
            elem->endX = 0.0f;
            elem->startXChange = 0.0f;
            elem->startX = 0.0f;
            elem->endXChange = elem->initialSpeed;
            elem->yaw = Rand_ZeroOne() * 65534.0f;
            elem->pitch = Rand_ZeroOne() * 65534.0f;
        }

        self->lightDecay = initParams->lightDecay;
        if (self->lightDecay == true) {
            self->lightInfo.type = LIGHT_POINT_NOGLOW;
            self->lightInfo.params.point = initParams->lightPoint;
            self->lightNode =
                LightContext_InsertLight(Effect_GetGlobalCtx(), &Effect_GetGlobalCtx()->lightCtx, &self->lightInfo);
        } else {
            self->lightNode = NULL;
        }
    }
}

void EffectShieldParticle_Destroy(void* thisx) {
    EffectShieldParticle* self = (EffectShieldParticle*)thisx;

    if ((self != NULL) && (self->lightDecay == true)) {
        if (self->lightNode == Effect_GetGlobalCtx()->lightCtx.listHead) {
            Effect_GetGlobalCtx()->lightCtx.listHead = self->lightNode->next;
        }
        LightContext_RemoveLight(Effect_GetGlobalCtx(), &Effect_GetGlobalCtx()->lightCtx, self->lightNode);
    }
}

s32 EffectShieldParticle_Update(void* thisx) {
    EffectShieldParticle* self = (EffectShieldParticle*)thisx;
    EffectShieldParticleElement* elem;

    if (self == NULL) {
        return 0;
    }

    for (elem = &self->elements[0]; elem < &self->elements[self->numElements]; elem++) {
        elem->endXChange -= self->deceleration;
        if (elem->endXChange < 0.0f) {
            elem->endXChange = 0.0f;
        }

        if (elem->startXChange > 0.0f) {
            elem->startXChange -= self->deceleration;
            if (elem->startXChange < 0.0f) {
                elem->startXChange = 0.0f;
            }
        }

        elem->endX += elem->endXChange;
        elem->startX += elem->startXChange;

        if ((elem->startXChange == 0.0f) && (self->lengthCutoff < (elem->endX - elem->startX))) {
            elem->startXChange = elem->initialSpeed;
        }
    }

    if (self->lightDecay == true) {
        self->lightInfo.params.point.radius /= 2;
    }

    self->timer++;

    if (self->duration < self->timer) {
        return 1;
    }

    return 0;
}

void EffectShieldParticle_GetColors(EffectShieldParticle* self, Color_RGBA8* primColor, Color_RGBA8* envColor) {
    s32 halfDuration = self->duration * 0.5f;
    f32 ratio;

    if (halfDuration == 0) {
        primColor->r = self->primColorStart.r;
        primColor->g = self->primColorStart.g;
        primColor->b = self->primColorStart.b;
        primColor->a = self->primColorStart.a;
        envColor->r = self->envColorStart.r;
        envColor->g = self->envColorStart.g;
        envColor->b = self->envColorStart.b;
        envColor->a = self->envColorStart.a;
    } else if (self->timer < halfDuration) {
        ratio = self->timer / (f32)halfDuration;
        primColor->r = self->primColorStart.r + (self->primColorMid.r - self->primColorStart.r) * ratio;
        primColor->g = self->primColorStart.g + (self->primColorMid.g - self->primColorStart.g) * ratio;
        primColor->b = self->primColorStart.b + (self->primColorMid.b - self->primColorStart.b) * ratio;
        primColor->a = self->primColorStart.a + (self->primColorMid.a - self->primColorStart.a) * ratio;
        envColor->r = self->envColorStart.r + (self->envColorMid.r - self->envColorStart.r) * ratio;
        envColor->g = self->envColorStart.g + (self->envColorMid.g - self->envColorStart.g) * ratio;
        envColor->b = self->envColorStart.b + (self->envColorMid.b - self->envColorStart.b) * ratio;
        envColor->a = self->envColorStart.a + (self->envColorMid.a - self->envColorStart.a) * ratio;
    } else {
        ratio = (self->timer - halfDuration) / (f32)halfDuration;
        primColor->r = self->primColorMid.r + (self->primColorEnd.r - self->primColorMid.r) * ratio;
        primColor->g = self->primColorMid.g + (self->primColorEnd.g - self->primColorMid.g) * ratio;
        primColor->b = self->primColorMid.b + (self->primColorEnd.b - self->primColorMid.b) * ratio;
        primColor->a = self->primColorMid.a + (self->primColorEnd.a - self->primColorMid.a) * ratio;
        envColor->r = self->envColorMid.r + (self->envColorEnd.r - self->envColorMid.r) * ratio;
        envColor->g = self->envColorMid.g + (self->envColorEnd.g - self->envColorMid.g) * ratio;
        envColor->b = self->envColorMid.b + (self->envColorEnd.b - self->envColorMid.b) * ratio;
        envColor->a = self->envColorMid.a + (self->envColorEnd.a - self->envColorMid.a) * ratio;
    }
}

void EffectShieldParticle_Draw(void* thisx, GraphicsContext* gfxCtx) {
    EffectShieldParticle* self = (EffectShieldParticle*)thisx;
    EffectShieldParticleElement* elem;
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;

    OPEN_DISPS(gfxCtx, "../z_eff_shield_particle.c", 272);

    if (self != NULL) {
        POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0x26);

        gDPSetCycleType(POLY_XLU_DISP++, G_CYC_2CYCLE);
        gDPPipeSync(POLY_XLU_DISP++);
        gSPTexture(POLY_XLU_DISP++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

        gDPLoadTextureBlock(POLY_XLU_DISP++, gUnknownCircle6Tex, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

        gDPSetCombineLERP(POLY_XLU_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL0, 0, 0, 0,
                          0, COMBINED, 0, 0, 0, COMBINED);
        gDPSetRenderMode(POLY_XLU_DISP++, G_RM_PASS, G_RM_ZB_CLD_SURF2);
        gSPClearGeometryMode(POLY_XLU_DISP++, G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR);
        gSPSetGeometryMode(POLY_XLU_DISP++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);

        EffectShieldParticle_GetColors(self, &primColor, &envColor);

        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, primColor.r, primColor.g, primColor.b, primColor.a);
        gDPSetEnvColor(POLY_XLU_DISP++, envColor.r, envColor.g, envColor.b, envColor.a);
        gDPPipeSync(POLY_XLU_DISP++);

        for (elem = &self->elements[0]; elem < &self->elements[self->numElements]; elem++) {
            Mtx* mtx;
            MtxF sp104;
            MtxF spC4;
            MtxF sp84;
            f32 temp1 = (s16)((elem->endX + elem->startX) * 0.5f);
            f32 temp2 = elem->endX - elem->startX;
            f32 temp3 = (s16)((temp2 * (1.0f / 64.0f)) / 0.02f);

            if (temp3 < 1.0f) {
                temp3 = 1.0f;
            }

            SkinMatrix_SetTranslate(&spC4, self->position.x, self->position.y, self->position.z);
            SkinMatrix_SetRotateRPY(&sp104, 0, elem->yaw, 0);
            SkinMatrix_MtxFMtxFMult(&spC4, &sp104, &sp84);
            SkinMatrix_SetRotateRPY(&sp104, 0, 0, elem->pitch);
            SkinMatrix_MtxFMtxFMult(&sp84, &sp104, &spC4);
            SkinMatrix_SetTranslate(&sp104, temp1, 0.0f, 0.0f);
            SkinMatrix_MtxFMtxFMult(&spC4, &sp104, &sp84);
            SkinMatrix_SetScale(&sp104, temp3 * 0.02f, 0.02f, 0.02f);
            SkinMatrix_MtxFMtxFMult(&sp84, &sp104, &spC4);

            mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &spC4);
            if (mtx == NULL) {
                break;
            }

            gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPVertex(POLY_XLU_DISP++, sVertices, 4, 0);
            gSP2Triangles(POLY_XLU_DISP++, 0, 1, 2, 0, 0, 3, 1, 0);
        }
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_shield_particle.c", 359);
}
