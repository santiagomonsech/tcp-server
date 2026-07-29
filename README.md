# tcp-server

> **Parte del roadmap:** Fase 1 · Track C (Systems Programming)
> **CS:APP:** Cap 11 — Network Programming
> **Lenguaje:** C

Un servidor HTTP/1.0 construido desde sockets raw en C — sin frameworks, sin libcurl del lado servidor. El objetivo es entender qué hace el kernel por vos en cada paso de `socket → bind → listen → accept`, y qué significa realmente "parsear" un protocolo de texto como HTTP.

---

## Milestones

- [ ] **M1:** Accept loop básico — el server escucha en un puerto, acepta una conexión, la cierra sin responder nada. Verificable con `nc localhost PUERTO`.
- [ ] **M2:** Parseo de una request HTTP/1.0 real — request line (`GET /path HTTP/1.0`) + headers mínimos.
- [ ] **M3:** Response mínima — `200 OK`, `Content-Length`, body fijo. Verificable con `curl localhost:PUERTO`.
- [ ] **M4 (final):** Manejar conexiones concurrentes (fork-per-connection) y poder explicar el trade-off contra un accept loop puramente secuencial — anclado a OSTEP módulo 1.1 (procesos y scheduling).

## Estructura

```
tcp-server/
├── README.md           ← este archivo
├── CONTEXT.md          ← vocabulario del dominio y decisiones de arquitectura
├── CLAUDE.md           ← instrucciones para Claude Code en este proyecto
├── CHANGELOG.md
├── src/                ← código fuente
├── tests/               ← tests
├── notes/               ← notas de sesión (generadas por /recap)
└── docs/
    └── architecture.md ← diseño del sistema, decisiones tomadas
```

## Cómo correr

Todavía no hay código — se arranca con M1. Una vez que exista un `Makefile`:

```bash
make           # build
make test      # build + run tests
make asan      # build con AddressSanitizer
```

## Comandos disponibles en este proyecto

| Comando | Descripción |
|---------|-------------|
| `/check` | Compila, corre tests, verifica con valgrind/sanitizers |
| `/explore` | Explora el sistema operativo relacionado con el tema activo (ej. `strace -e trace=network`, `ss -tlnp`) |
| `/next` | Próximo paso concreto del proyecto |
| `/recap` | Escribe notas de la sesión actual en notes/ |

## Agentes útiles para este proyecto

- `@debugger` — cuando algo explota, gdb/strace guiado
- `@reviewer` — para revisar el código en cualquier momento
- `@quiz` — para testear comprensión después de un milestone
- `@explainer` — cuando un concepto no está claro

## Reference

- Bryant & O'Hallaron — *Computer Systems: A Programmer's Perspective*, 3e · Chapter 11
- Kurose & Ross — *Computer Networking: A Top-Down Approach*
- RFC 1945 — Hypertext Transfer Protocol (HTTP/1.0)
