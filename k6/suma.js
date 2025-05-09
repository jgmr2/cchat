import http from 'k6/http';
import { check } from 'k6';

export const options = {
  vus: 150,             // usuarios virtuales (concurrentes)
  iterations: 1000,  // total de solicitudes
};

export default function () {
  const a = Math.floor(Math.random() * 1000);
  const b = Math.floor(Math.random() * 1000);

  const url = `http://localhost/auth/add/${a}/${b}`;
  const res = http.get(url);

  check(res, {
    'status is 200': (r) => r.status === 200,
  });
}
