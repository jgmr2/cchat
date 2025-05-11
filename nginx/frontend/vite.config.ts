import tailwindcss from '@tailwindcss/vite';
import { sveltekit } from '@sveltejs/kit/vite';
import { defineConfig } from 'vite';
import adapter from '@sveltejs/adapter-static';

export default defineConfig({
    plugins: [tailwindcss(), sveltekit()],
    kit: {
        adapter: adapter({
            pages: 'build', // Directorio donde se generará el build
            assets: 'build', // Directorio para los archivos estáticos
            fallback: 'index.html', // Fallback para SPA
        }),
    },
});