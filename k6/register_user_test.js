import http from 'k6/http';
import { check, sleep } from 'k6';

export const options = {
    vus: 10, // Usuarios concurrentes
    duration: '10s', // Duración de la prueba
};

export default function () {
    const url = 'http://localhost/auth/register';
    const payload = `username=user${__VU}&email=user${__VU}@test.com&password=secret`;

    const headers = {
        'Content-Type': 'application/x-www-form-urlencoded',
    };

    const res = http.post(url, payload, { headers });

    check(res, {
        'status is 200': (r) => r.status === 200,
        'body is not empty': (r) => r.body.length > 0,
    });

    sleep(1); // esperar 1 segundo entre usuarios
}
