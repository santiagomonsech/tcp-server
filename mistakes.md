# Mistakes — tcp-server

Errores conceptuales y cómo se corrigieron.
El @reviewer los agrega automáticamente. Son el mejor material de repaso.

---

### 2026-08-25 — Confundir tracking de conexiones concurrentes (M4) con el accept loop secuencial (M1)
**El error:** Agregar un array `peer_connections[ACTIVE_CONNS]` en el accept loop de M1 "para saber cuáles hay que cerrar cuando vaya a apagar el servidor" — es decir, creer que el modelo secuencial (una conexión a la vez) necesita un registro de fds vivos para poder cerrarlos después.
**La realidad:** En un accept loop puramente secuencial no hace falta trackear nada — cada conexión se maneja y se cierra (`close(client_fd)`) antes de volver a llamar a `accept()`, así que nunca hay más de un fd de conexión abierto a la vez. Un registro de conexiones vivas solo tiene sentido cuando hay múltiples conexiones abiertas simultáneamente (fork-per-connection u otro modelo concurrente), que es exactamente el problema de M4, no de M1. Como consecuencia del error, el array nunca se cerraba individualmente y el loop terminaba con `break` al llegar a `ACTIVE_CONNS` conexiones en vez de seguir aceptando indefinidamente — un leak de file descriptors disfrazado de feature.
**Cómo se detectó:** Code review (@reviewer) — el array se llenaba pero nunca se leía ni se usaba para cerrar nada (no había signal handler ni lógica de shutdown que lo consultara), y el `close(client_fd)` que pide el scope de M1 no aparecía en ningún lado del loop.
**Pregunta para recordarlo:** en un accept loop estrictamente secuencial (una conexión completa antes de aceptar la siguiente), ¿en qué momento podés tener dos file descriptors de conexión abiertos al mismo tiempo? Si la respuesta es "nunca", ¿para qué necesitarías un array para trackearlos?
