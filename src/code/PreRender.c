#include "global.h"
#include "alloca.h"

void PreRender_SetValuesSave(PreRender* self, u32 width, u32 height, void* fbuf, void* zbuf, void* cvg) {
    self->widthSave = width;
    self->heightSave = height;
    self->fbufSave = fbuf;
    self->cvgSave = cvg;
    self->zbufSave = zbuf;
    self->ulxSave = 0;
    self->ulySave = 0;
    self->lrxSave = width - 1;
    self->lrySave = height - 1;
}

void PreRender_Init(PreRender* self) {
    bzero(self, sizeof(PreRender));
    ListAlloc_Init(&self->alloc);
}

void PreRender_SetValues(PreRender* self, u32 width, u32 height, void* fbuf, void* zbuf) {
    self->width = width;
    self->height = height;
    self->fbuf = fbuf;
    self->zbuf = zbuf;
    self->ulx = 0;
    self->uly = 0;
    self->lrx = width - 1;
    self->lry = height - 1;
}

void PreRender_Destroy(PreRender* self) {
    ListAlloc_FreeAll(&self->alloc);
}

void func_800C0F28(PreRender* self, Gfx** gfxp, void* buf, void* bufSave) {
    Gfx* gfx;
    s32 x;
    s32 x2;
    s32 dx;

    LogUtils_CheckNullPointer("self", self, "../PreRender.c", 215);
    LogUtils_CheckNullPointer("glistpp", gfxp, "../PreRender.c", 216);
    gfx = *gfxp;
    LogUtils_CheckNullPointer("glistp", gfx, "../PreRender.c", 218);

    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++,
                    G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_CONV | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                        G_TD_CLAMP | G_TP_NONE | G_CYC_COPY | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_NOOP2);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, self->width, bufSave);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, self->width, self->height);

    dx = 0x1000 / (self->width * 2);

    x = self->height;
    x2 = 0;
    while (x > 0) {
        s32 uls = 0;
        s32 lrs = self->width - 1;
        s32 ult;
        s32 lrt;

        dx = CLAMP_MAX(dx, x);
        ult = x2;
        lrt = (ult + dx) - 1;

        gDPLoadTextureTile(gfx++, buf, G_IM_FMT_RGBA, G_IM_SIZ_16b, self->width, self->height, uls, ult, lrs, lrt, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                           G_TX_NOLOD);
        gSPTextureRectangle(gfx++, uls << 2, ult << 2, lrs << 2, lrt << 2, G_TX_RENDERTILE, uls << 5, ult << 5, 4 << 10,
                            1 << 10);

        x -= dx;
        x2 += dx;
    }

    gDPPipeSync(gfx++);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, self->width, self->fbuf);
    *gfxp = gfx;
}

void func_800C1258(PreRender* self, Gfx** gfxp) {
    Gfx* gfx;
    s32 y;
    s32 y2;
    s32 dy;

    LogUtils_CheckNullPointer("self", self, "../PreRender.c", 278);
    LogUtils_CheckNullPointer("glistpp", gfxp, "../PreRender.c", 279);
    gfx = *gfxp;
    LogUtils_CheckNullPointer("glistp", gfx, "../PreRender.c", 281);

    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++,
                    G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_CONV | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                        G_TD_CLAMP | G_TP_NONE | G_CYC_COPY | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_NOOP2);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, self->width, self->fbuf);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, self->ulx, self->uly, self->lrx + 1, self->lry + 1);

    dy = 0x1000 / ((self->lrxSave - self->ulxSave + 1) * 2);

    y = (self->lrySave - self->ulySave) + 1;
    y2 = 0;
    while (y > 0) {
        s32 ult;
        s32 lrt;
        s32 uly;

        dy = CLAMP_MAX(dy, y);

        ult = self->ulySave + y2;
        lrt = (ult + dy) - 1;
        uly = self->uly + y2;

        gDPLoadTextureTile(gfx++, self->fbufSave, G_IM_FMT_RGBA, G_IM_SIZ_16b, self->widthSave, self->height - 1,
                           self->ulxSave, ult, self->lrxSave, lrt, 0, G_TX_NOMIRROR | G_TX_WRAP,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(gfx++, self->ulx << 2, uly << 2, self->lrx << 2, (uly + dy - 1) << 2, G_TX_RENDERTILE,
                            self->ulxSave << 5, ult << 5, 4 << 10, 1 << 10);

        y -= dy;
        y2 += dy;
    }

    gDPPipeSync(gfx++);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, self->width, self->fbuf);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, self->width, self->height);
    *gfxp = gfx;
}

void func_800C170C(PreRender* self, Gfx** gfxp, void* fbuf, void* fbufSave, u32 r, u32 g, u32 b, u32 a) {
    Gfx* gfx;
    s32 x;
    s32 x2;
    s32 dx;

    LogUtils_CheckNullPointer("self", self, "../PreRender.c", 343);
    LogUtils_CheckNullPointer("glistpp", gfxp, "../PreRender.c", 344);
    gfx = *gfxp;
    LogUtils_CheckNullPointer("glistp", gfx, "../PreRender.c", 346);

    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++,
                    G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                        G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PRIM | G_RM_OPA_SURF | G_RM_OPA_SURF2);
    gDPSetEnvColor(gfx++, r, g, b, a);
    gDPSetCombineLERP(gfx++, 0, 0, 0, TEXEL0, 0, 0, 0, 1, 0, 0, 0, TEXEL0, 0, 0, 0, 1);
    gDPSetCombineLERP(gfx++, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, 1, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, 1);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, self->width, fbufSave);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, self->width, self->height);

    dx = 0x1000 / (self->width * 2);

    x = self->height;
    x2 = 0;
    while (x > 0) {
        s32 uls = 0;
        s32 lrs = self->width - 1;
        s32 ult;
        s32 lrt;

        dx = CLAMP_MAX(dx, x);
        ult = x2;
        lrt = x2 + dx - 1;

        gDPLoadTextureTile(gfx++, fbuf, G_IM_FMT_RGBA, G_IM_SIZ_16b, self->width, self->height, uls, ult, lrs, lrt, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                           G_TX_NOLOD);

        gSPTextureRectangle(gfx++, uls << 2, ult << 2, (lrs + 1) << 2, (lrt + 1) << 2, G_TX_RENDERTILE, uls << 5,
                            ult << 5, 1 << 10, 1 << 10);

        x -= dx;
        x2 += dx;
    }

    gDPPipeSync(gfx++);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, self->width, self->fbuf);
    *gfxp = gfx;
}

void func_800C1AE8(PreRender* self, Gfx** gfxp, void* fbuf, void* fbufSave) {
    func_800C170C(self, gfxp, fbuf, fbufSave, 255, 255, 255, 255);
}

void func_800C1B24(PreRender* self, Gfx** gfxp, void* fbuf, void* cvgSave) {
    Gfx* gfx;
    s32 x;
    s32 x2;
    s32 dx;

    LogUtils_CheckNullPointer("self", self, "../PreRender.c", 422);
    LogUtils_CheckNullPointer("glistpp", gfxp, "../PreRender.c", 423);
    gfx = *gfxp;
    LogUtils_CheckNullPointer("glistp", gfx, "../PreRender.c", 425);

    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++,
                    G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                        G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PRIM | G_RM_PASS | G_RM_OPA_CI2);
    gDPSetCombineLERP(gfx++, 0, 0, 0, TEXEL0, 0, 0, 0, 0, 0, 0, 0, TEXEL0, 0, 0, 0, 0);
    gDPSetColorImage(gfx++, G_IM_FMT_I, G_IM_SIZ_8b, self->width, cvgSave);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, self->width, self->height);

    dx = 0x1000 / (self->width * 2);

    x = self->height;
    x2 = 0;
    while (x > 0) {
        s32 uls = 0;
        s32 lrs = self->width - 1;
        s32 ult;
        s32 lrt;

        dx = CLAMP_MAX(dx, x);
        ult = x2;
        lrt = x2 + dx - 1;

        gDPLoadTextureTile(gfx++, fbuf, G_IM_FMT_IA, G_IM_SIZ_16b, self->width, self->height, uls, ult, lrs, lrt, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                           G_TX_NOLOD);

        gSPTextureRectangle(gfx++, uls << 2, ult << 2, (lrs + 1) << 2, (lrt + 1) << 2, G_TX_RENDERTILE, uls << 5,
                            ult << 5, 1 << 10, 1 << 10);
        x -= dx;
        x2 += dx;
    }

    gDPPipeSync(gfx++);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, self->width, self->fbuf);
    *gfxp = gfx;
}

void func_800C1E9C(PreRender* self, Gfx** gfxp) {
    LogUtils_CheckNullPointer("self->zbuf_save", self->zbufSave, "../PreRender.c", 481);
    LogUtils_CheckNullPointer("self->zbuf", self->zbuf, "../PreRender.c", 482);

    if ((self->zbufSave != NULL) && (self->zbuf != NULL)) {
        func_800C0F28(self, gfxp, self->zbuf, self->zbufSave);
    }
}

void func_800C1F20(PreRender* self, Gfx** gfxp) {
    LogUtils_CheckNullPointer("self->fbuf_save", self->fbufSave, "../PreRender.c", 495);
    LogUtils_CheckNullPointer("self->fbuf", self->fbuf, "../PreRender.c", 496);

    if ((self->fbufSave != NULL) && (self->fbuf != NULL)) {
        func_800C1AE8(self, gfxp, self->fbuf, self->fbufSave);
    }
}

void func_800C1FA4(PreRender* self, Gfx** gfxp) {
    Gfx* gfx = *gfxp;

    gDPPipeSync(gfx++);
    gDPSetBlendColor(gfx++, 255, 255, 255, 8);
    gDPSetPrimDepth(gfx++, -1, -1);
    gDPSetOtherMode(gfx++,
                    G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                        G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PRIM | G_RM_VISCVG | G_RM_VISCVG2);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, self->width, self->height);
    gDPFillRectangle(gfx++, 0, 0, self->width, self->height);
    gDPPipeSync(gfx++);

    *gfxp = gfx;
}

void func_800C20B4(PreRender* self, Gfx** gfxp) {
    func_800C1FA4(self, gfxp);
    LogUtils_CheckNullPointer("self->cvg_save", self->cvgSave, "../PreRender.c", 532);
    if (self->cvgSave != NULL) {
        func_800C1B24(self, gfxp, self->fbuf, self->cvgSave);
    }
}

void func_800C2118(PreRender* self, Gfx** gfxp) {
    func_800C0F28(self, gfxp, self->zbufSave, self->zbuf);
}

void func_800C213C(PreRender* self, Gfx** gfxp) {
    Gfx* gfx;
    s32 y;
    s32 y2;
    s32 dy;
    s32 rtile = 1;

    if (self->cvgSave != NULL) {
        LogUtils_CheckNullPointer("self", self, "../PreRender.c", 563);
        LogUtils_CheckNullPointer("glistpp", gfxp, "../PreRender.c", 564);
        gfx = *gfxp;
        LogUtils_CheckNullPointer("glistp", gfx, "../PreRender.c", 566);

        gDPPipeSync(gfx++);
        gDPSetEnvColor(gfx++, 255, 255, 255, 32);
        gDPSetOtherMode(gfx++,
                        G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                            G_TD_CLAMP | G_TP_NONE | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                        G_AC_NONE | G_ZS_PRIM | AA_EN | CVG_DST_CLAMP | ZMODE_OPA | CVG_X_ALPHA |
                            GBL_c1(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1) |
                            GBL_c2(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1));
        gDPSetCombineLERP(gfx++, 0, 0, 0, TEXEL0, 1, 0, TEXEL1, ENVIRONMENT, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);

        dy = 4;

        y = self->height;
        y2 = 0;
        while (y > 0) {
            s32 uls = 0;
            s32 lrs = self->width - 1;
            s32 ult;
            s32 lrt;

            dy = CLAMP_MAX(dy, y);

            ult = y2;
            lrt = (y2 + dy - 1);

            gDPLoadMultiTile(gfx++, self->fbufSave, 0x0000, G_TX_RENDERTILE, G_IM_FMT_RGBA, G_IM_SIZ_16b, self->width,
                             self->height, uls, ult, lrs, lrt, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                             G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

            gDPLoadMultiTile(gfx++, self->cvgSave, 0x0160, rtile, G_IM_FMT_I, G_IM_SIZ_8b, self->width, self->height,
                             uls, ult, lrs, lrt, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                             G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

            gSPTextureRectangle(gfx++, uls << 2, ult << 2, (lrs + 1) << 2, (lrt + 1) << 2, G_TX_RENDERTILE, uls << 5,
                                ult << 5, 1 << 10, 1 << 10);

            y -= dy;
            y2 += dy;
        }

        gDPPipeSync(gfx++);
        *gfxp = gfx;
    }
}

void func_800C24BC(PreRender* self, Gfx** gfxp) {
    func_800C0F28(self, gfxp, self->fbufSave, self->fbuf);
}

void func_800C24E0(PreRender* self, Gfx** gfxp) {
    func_800C1258(self, gfxp);
}

void func_800C2500(PreRender* self, s32 x, s32 y) {
    s32 i;
    s32 j;
    s32 buffA[3 * 5];
    s32 buffR[3 * 5];
    s32 buffG[3 * 5];
    s32 buffB[3 * 5];
    s32 x1;
    s32 y1;
    s32 pad;
    s32 pxR;
    s32 pxG;
    s32 pxB;
    s32 pxR2;
    s32 pxG2;
    s32 pxB2;
    Color_RGBA16 pxIn;
    Color_RGBA16 pxOut;
    u32 pxR3;
    u32 pxG3;
    u32 pxB3;

    /*
    Picture self as a 3x5 rectangle where the middle pixel (index 7) correspond to (x, y)
      _ _ _ _ _
    | 0 1 2 3 4 |
    | 5 6 7 8 9 |
    | A B C D E |
      ‾ ‾ ‾ ‾ ‾
    */
    for (i = 0; i < 3 * 5; i++) {
        x1 = (i % 5) + x - 2;
        y1 = (i / 5) + y - 1;

        if (x1 < 0) {
            x1 = 0;
        } else if (x1 > (self->width - 1)) {
            x1 = self->width - 1;
        }
        if (y1 < 0) {
            y1 = 0;
        } else if (y1 > (self->height - 1)) {
            y1 = self->height - 1;
        }

        pxIn.rgba = self->fbufSave[x1 + y1 * self->width];
        buffR[i] = (pxIn.r << 3) | (pxIn.r >> 2);
        buffG[i] = (pxIn.g << 3) | (pxIn.g >> 2);
        buffB[i] = (pxIn.b << 3) | (pxIn.b >> 2);
        buffA[i] = self->cvgSave[x1 + y1 * self->width] >> 5; // A
    }

    if (buffA[7] == 7) {
        osSyncPrintf("Error, should not be in here \n");
        return;
    }

    pxR = pxR2 = buffR[7];
    pxG = pxG2 = buffG[7];
    pxB = pxB2 = buffB[7];

    for (i = 1; i < 3 * 5; i += 2) {
        if (buffA[i] == 7) {
            if (pxR < buffR[i]) {
                for (j = 1; j < 15; j += 2) {
                    if ((i != j) && (buffR[j] >= buffR[i]) && (buffA[j] == 7)) {
                        pxR = buffR[i];
                    }
                }
            }
            if (pxG < buffG[i]) {
                for (j = 1; j < 15; j += 2) {
                    if ((i != j) && (buffG[j] >= buffG[i]) && (buffA[j] == 7)) {
                        pxG = buffG[i];
                    }
                }
            }
            if (pxB < buffB[i]) {
                for (j = 1; j < 15; j += 2) {
                    if ((i != j) && (buffB[j] >= buffB[i]) && (buffA[j] == 7)) {
                        pxB = buffB[i];
                    }
                }
            }
            if (1) {}
            if (pxR2 > buffR[i]) {
                for (j = 1; j < 15; j += 2) {
                    if ((i != j) && (buffR[j] <= buffR[i]) && (buffA[j] == 7)) {
                        pxR2 = buffR[i];
                    }
                }
            }
            if (pxG2 > buffG[i]) {
                for (j = 1; j < 15; j += 2) {
                    if ((i != j) && (buffG[j] <= buffG[i]) && (buffA[j] == 7)) {
                        pxG2 = buffG[i];
                    }
                }
            }
            if (pxB2 > buffB[i]) {
                for (j = 1; j < 15; j += 2) {
                    if ((i != j) && (buffB[j] <= buffB[i]) && (buffA[j] == 7)) {
                        pxB2 = buffB[i];
                    }
                }
            }
        }
    }

    pxR3 = buffR[7] + ((s32)((7 - buffA[7]) * ((pxR + pxR2) - (buffR[7] << 1)) + 4) >> 3);
    pxG3 = buffG[7] + ((s32)((7 - buffA[7]) * ((pxG + pxG2) - (buffG[7] << 1)) + 4) >> 3);
    pxB3 = buffB[7] + ((s32)((7 - buffA[7]) * ((pxB + pxB2) - (buffB[7] << 1)) + 4) >> 3);

    pxOut.r = pxR3 >> 3;
    pxOut.g = pxG3 >> 3;
    pxOut.b = pxB3 >> 3;
    pxOut.a = 1;
    self->fbufSave[x + y * self->width] = pxOut.rgba;
}

void func_800C2FE4(PreRender* self) {
    s32 x;
    s32 y;
    s32 phi_v0;
    u8* buffR = alloca(self->width);
    u8* buffG = alloca(self->width);
    u8* buffB = alloca(self->width);
    s32 pad[3];
    s32 pxR;
    s32 pxG;
    s32 pxB;

    for (y = 0; y < self->height; y++) {
        for (x = 0; x < self->width; x++) {
            Color_RGBA16 pxIn;

            pxIn.rgba = self->fbufSave[x + y * self->width];
            buffR[x] = pxIn.r;
            buffG[x] = pxIn.g;
            buffB[x] = pxIn.b;
        }

        for (x = 1; x < self->width - 1; x++) {
            Color_RGBA16 pxOut;
            s32 a = self->cvgSave[x + y * self->width];

            a >>= 5;
            if (a == 7) {
                continue;
            }

            if (((HREG(80) == 0xF) ? HREG(81) : 0) != 0) {
                if (((HREG(80) == 0xF) ? HREG(81) : 0) != 0) {}

                if (((HREG(80) == 0xF) ? HREG(81) : 0) == 5) {
                    pxR = 31;
                    pxG = 0;
                    pxB = 0;
                } else {
                    u8* temp_s0 = &buffR[x - 1];
                    u8* temp_s1 = &buffG[x - 1];
                    u8* temp_s2 = &buffB[x - 1];

                    if (((HREG(80) == 0xF) ? HREG(81) : 0) == 3) {
                        osSyncPrintf("red=%3d %3d %3d %3d grn=%3d %3d %3d %3d blu=%3d %3d %3d %3d \n", temp_s0[0],
                                     temp_s0[1], temp_s0[2], MEDIAN3(temp_s0[0], temp_s0[1], temp_s0[2]), temp_s1[0],
                                     temp_s1[1], temp_s1[2], MEDIAN3(temp_s1[0], temp_s1[1], temp_s1[2]), temp_s2[0],
                                     temp_s2[1], temp_s2[2], MEDIAN3(temp_s2[0], temp_s2[1], temp_s2[2]));
                    }

                    if (((HREG(80) == 0xF) ? HREG(81) : 0) == 1) {
                        pxR = MEDIAN3(temp_s0[0], temp_s0[1], temp_s0[2]);
                        pxG = MEDIAN3(temp_s1[0], temp_s1[1], temp_s1[2]);
                        pxB = MEDIAN3(temp_s2[0], temp_s2[1], temp_s2[2]);
                    } else {
                        pxR = MEDIAN3(temp_s0[0], temp_s0[1], temp_s0[2]);
                        pxG = MEDIAN3(temp_s1[0], temp_s1[1], temp_s1[2]);
                        pxB = MEDIAN3(temp_s2[0], temp_s2[1], temp_s2[2]);
                    }
                }
                pxOut.r = pxR;
                pxOut.g = pxG;
                pxOut.b = pxB;
                pxOut.a = 1;
            }
            self->fbufSave[x + y * self->width] = pxOut.rgba;
        }
    }
}

void PreRender_Calc(PreRender* self) {
    s32 x;
    s32 y;

    if ((self->cvgSave != NULL) && (self->fbufSave != NULL)) {

        for (y = 0; y < self->height; y++) {
            for (x = 0; x < self->width; x++) {
                s32 a = self->cvgSave[x + y * self->width];
                a >>= 5;
                a++;
                if (a != 8) {
                    //func_800C2500(self, x, y);
                }
            }
        }

        if (HREG(80) == 0xF ? HREG(81) : 0) {
            func_800C2FE4(self);
        }
    }
}
