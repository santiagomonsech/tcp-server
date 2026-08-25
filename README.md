# tcp-server

An HTTP/1.0 server built from raw sockets in C — no frameworks, no libcurl on the server side.

---

## Why I built this

I want to understand what actually happens between a raw TCP connection and an HTTP response — what the kernel does at each step of `socket → bind → listen → accept`, and what "parsing" really means for a text protocol like HTTP, without a framework doing it for me.

---

## How it works

Every HTTP/1.0 request goes through the same fixed pipeline: a listening socket accepts a new connection, the request is read and parsed into a request line plus headers, a response is built and written back, and the connection is closed. Sockets, parsing and formatting are all done by hand so it's clear exactly what a framework like Flask/Express replaces when you don't think twice about using it.

### Accept loop

The server creates a listening socket (`socket` → `bind` → `listen`), then blocks on `accept` waiting for an incoming connection. Each `accept` call returns a *new* file descriptor for that specific connection — the original listening socket keeps waiting for the next one. In the first milestone this loop is sequential: one connection is fully handled before `accept` is called again.

### Request parsing

Once a connection is accepted, the server reads raw bytes off the socket until it has a full HTTP/1.0 request: a request line (`GET /path HTTP/1.0`) followed by a minimal set of headers, terminated by `\r\n\r\n`. There's no assumption that a single `read()` call returns the whole request — TCP is a byte stream, not a message protocol, so parsing has to handle a request arriving in pieces.

### Response writing

The server writes back a status line (`HTTP/1.0 200 OK`), a `Content-Length` header, and a fixed body. HTTP/1.0 has no chunked encoding, so `Content-Length` is what lets the client know exactly when the response ends — and no keep-alive, so the connection closes right after.

```
                 +--------------+
  TCP connection | accept loop  |  <- socket/bind/listen/accept
                 +------+-------+
                        | connection fd
                        v
                 +--------------+
                 | request      |  <- read() until \r\n\r\n,
                 | parser       |     parse request line + headers
                 +------+-------+
                        | struct http_request
                        v
                 +--------------+
                 | response     |  <- status line + Content-Length + body
                 | writer       |
                 +------+-------+
                        |
                        v
                    close(fd)
```

---

## Design decisions

**Why blocking I/O instead of epoll/select from the start?**
Because the goal of M1-M3 is to understand what a single connection actually requires at the syscall level — one `accept`, one `read`, one `write`, one `close` — before adding the complexity of multiplexing. Non-blocking I/O and event loops are a separate topic (Phase 2 of the curriculum, `redis-lite`), not something to bring in early just because it's "more correct" in production.

**Why HTTP/1.0 instead of 1.1?**
HTTP/1.0 has no keep-alive and no chunked transfer encoding, which means every request/response cycle is a clean, self-contained unit: one connection, one request, one response, `Content-Length` always known up front. That's the smallest version of the protocol that's still real HTTP, which makes it the right starting point before adding the extra state 1.1 needs (persistent connections, chunked bodies).

---

## Current status

| Milestone | Description | Status |
|-----------|-------------|--------|
| M1 | Basic accept loop — listens on a port, accepts one connection, closes it without responding | Completed |
| M2 | Parse a real HTTP/1.0 request — request line + minimal headers | ⬜ Planned |
| M3 | Minimal response — `200 OK`, `Content-Length`, fixed body | ⬜ Planned |
| M4 | Concurrent connections (fork-per-connection) vs. a purely sequential accept loop | ⬜ Planned |

---

## Build and run

No code yet — starts with M1. Once a `Makefile` exists:

```bash
make           # build
make test      # build + run tests
make asan      # build with AddressSanitizer
```

**Requirements:** gcc

---

## Reference

- Bryant & O'Hallaron — *Computer Systems: A Programmer's Perspective*, 3e · Chapter 11
- Kurose & Ross — *Computer Networking: A Top-Down Approach*
- RFC 1945 — Hypertext Transfer Protocol (HTTP/1.0)
