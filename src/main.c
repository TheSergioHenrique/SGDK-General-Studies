#include <genesis.h>
#include "resources.h"

// Frequencias em Hz (temperamento igual, A4 = 440)
#define N_D4   294
#define N_E4   330
#define N_FS4  370   // F#4
#define N_G4   392
#define N_A4   440
#define N_B4   494
#define N_C5   523
#define N_D5   587
#define REST     0

// Duracoes em frames (NTSC = 60 fps). 1 tempo (quarter) = 40 frames -> 90 BPM.
#define EI     20              // eighth
#define TR     13              // triplet eighth
#define Q      40              // quarter
#define H      (Q * 2)         // half
#define DH     (Q * 3)         // dotted half = compasso inteiro em 3/4
#define LONG4  (Q * 4)
#define LONG5  (Q * 5)         // half + dotted half (cobre a pausa do "do")

typedef struct {
    u16 freq;
    u16 frames;
} Note;

MIDI de referência.
static const Note melody[] = {
    // Dai  sy        Dai  sy
    { N_D5,  DH }, { N_B4,  DH },
    { N_G4,  DH }, { N_D4,  H }, { REST, Q },
    // give  me  your  an swer  do
    { N_E4,  Q  }, { N_FS4, Q  }, { N_G4,  Q  },
    { N_E4,  H  }, { N_G4,  Q  },
    { N_D4,  LONG5 },
    // I'm  half  cra-zy,
    { N_A4,  DH }, { N_D5,  DH },
    { N_B4,  DH }, { N_G4,  H }, { REST, Q },
    // all  for  the  love  of  you
    { N_E4,  Q  }, { N_FS4, Q  }, { N_G4,  Q  },
    { N_A4,  H  }, { N_B4,  Q  },
    { N_A4,  LONG4 },
    // -----------------------------------------------------------------
    // It  won't  be   a   sty - lish  mar - - - riage,
    { N_B4,  Q  }, { N_C5,  Q  }, { N_B4,  Q  }, { N_A4,  Q  },
    { N_D5,  H  }, { N_B4,  Q  },
    { N_A4,  TR }, { N_B4,  TR }, { N_A4,  TR },
    { N_G4,  LONG4 },
    // I  can't   af - ford   a   car - riage
    { N_A4,  Q  }, { N_B4,  H  },
    { N_G4,  Q  }, { N_E4,  H  },
    { N_G4,  Q  }, { N_E4,  Q  }, { N_D4,  DH },
    { REST,  Q  },
    // But  you'll  look  sweet
    { N_D4,  Q  }, { N_G4,  H  },
    { N_B4,  Q  }, { N_A4,  Q  },
    // u - pon  the  seat
    { N_G4,  H  }, { N_B4,  Q  }, { N_A4,  Q  },
    // "(of  a)  bi - cy - cle   built   for   two."
    { N_B4,  EI }, { N_C5,  EI },
    { N_D5,  Q  }, { N_B4,  Q  }, { N_G4,  Q  },
    { N_A4,  H  }, { N_D4,  Q  },
    { N_G4,  LONG4 },

    { REST,  DH },
};

#define MELODY_LEN  (sizeof(melody) / sizeof(Note))

int main(bool hard) {
    // Konata como background no plano B.
    // - PAL1 -> reserva PAL0 pra fonte branca do SGDK
    // - TILE_USER_INDEX -> tiles da imagem entram depois dos tiles da fonte
    // - loadpal=TRUE  -> carrega a paleta da imagem em PAL1 automaticamente
    // - dma=TRUE      -> transferencia rapida dos tiles pra VRAM
    VDP_drawImageEx(BG_B, &bg_konata,
                    TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, TILE_USER_INDEX),
                    0, 0, TRUE, TRUE);

    VDP_drawText("Daisy Bell - Mega Drive PSG", 6, 22);
    VDP_drawText("github.com/TheSergioHenrique", 6, 24);

    u16 idx = 0;

    while (TRUE) {
        const Note* n = &melody[idx];

        if (n->freq == REST) {
            PSG_setEnvelope(0, 15);
        } else {
            PSG_setFrequency(0, n->freq);
            PSG_setEnvelope(0, 5);
        }

        // espera a duracao da nota
        for (u16 f = 0; f < n->frames; f++) {
            //espaço nas notas
            if (f == n->frames - 4 && n->freq != REST) {
                PSG_setEnvelope(0, 15);
            }
            SYS_doVBlankProcess();
        }

        idx = (idx + 1) % MELODY_LEN;
    }

    return 0;
}
