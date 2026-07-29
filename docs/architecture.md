# Arquitectura — tcp-server

Documento vivo. Se actualiza a medida que se toman decisiones.

---

## Visión del sistema

Servidor HTTP/1.0 desde sockets raw en C. Sin frameworks: cada capa (socket, parsing de texto, formateo de response) se escribe a mano para entender qué reemplaza exactamente un framework como Flask/Express cuando lo usás sin pensarlo dos veces.

Arranca como un accept loop secuencial bloqueante (una conexión a la vez) y termina el milestone final manejando conexiones concurrentes vía `fork()`, para poder comparar directamente el costo de ambos modelos.

---

## Diagrama de componentes (planeado)

```
                 ┌──────────────┐
  TCP connection │ accept loop  │ ← socket/bind/listen/accept
                 └──────┬───────┘
                        │ fd de la conexión
                        ▼
                 ┌──────────────┐
                 │ request      │ ← read() hasta \r\n\r\n, parseo de
                 │ parser       │   request line + headers mínimos
                 └──────┬───────┘
                        │ struct http_request
                        ▼
                 ┌──────────────┐
                 │ response     │ ← status line + Content-Length + body
                 │ writer       │
                 └──────┬───────┘
                        │
                        ▼
                    close(fd)
```

---

## Interfaces clave

_Pendiente — se completa cuando exista la primera firma de función real (M1)._

---

## Decisiones de diseño

_Ninguna todavía. Formato para cuando se tome la primera:_

### [FECHA] — [Título de la decisión]

**Alternativas consideradas:**
- [opción A]: [pros / contras]
- [opción B]: [pros / contras]

**Decisión:** [opción elegida]
**Razón:** [por qué]

---

## Invariantes

_Pendiente hasta M1._

---

## Pendientes / preguntas abiertas

- [ ] Blocking I/O simple vs. introducir non-blocking antes de tiempo (ver `CONTEXT.md`)
- [ ] Estrategia de lectura del request (buffer fijo vs. incremental)
