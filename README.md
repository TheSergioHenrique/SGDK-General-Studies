# Daisy Bell-SGDK

Toca a melodia de Daisy Bell no canal PSG do Mega Drive, com a Konata de fundo.
Feito em SGDK pra brincar com chiptune e quem sabe algum projeto futuro com o Kit.

## Build

```bash
podman run --rm --user 0:0 -v "$PWD":/src:Z ghcr.io/stephane-d/sgdk:latest
```

ROM sai em `out/rom.bin`. Roda em qualquer emulador de Mega Drive (Kega, BlastEm, RetroArch com core Genesis Plus GX).

Daisy Bell foi a primeira música cantada por um computador (IBM 704, 1961) e também foi cantada por HAL 9000 em 2001, do Stanley Kubrick.
