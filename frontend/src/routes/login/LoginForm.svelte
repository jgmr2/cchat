<form class="login-form" on:submit|preventDefault={handleLogin}>
    <label for="email">Email:</label>
    <input type="email" id="email" name="email" bind:value={email} placeholder="Email" />

    <label for="password">Password:</label>
    <input type="password" id="password" name="password" bind:value={password} placeholder="Password" />

    <button type="submit">Login</button>
</form>

<script>
    import { goto } from '$app/navigation';

    let email = '';
    let password = '';

    async function handleLogin() {
        try {
            const response = await fetch('http://localhost:81/auth/login', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ email, password }) // Cambiado a enviar email en lugar de username
            });

            if (response.ok) {
                const data = await response.json();
                console.log('Login successful:', data);

                // Almacenar el token en localStorage
                if (data.token) {
                    localStorage.setItem('sessionToken', data.token);
                }

                // Redirigir al usuario después del login exitoso
                goto('/'); // Cambia '/dashboard' por la ruta deseada
            } else {
                const error = await response.json();
                console.error('Login failed:', error.message);
                alert('Login failed: ' + error.message);
            }
        } catch (err) {
            console.error('Error connecting to the API:', err);
            alert('An error occurred. Please try again.');
        }
    }
</script>

<style>
    .login-form {
        display: flex;
        flex-direction: column;
        gap: 6px;
    }

    label {
        font-size: 13px;
        color: #444;
    }

    input {
        padding: 6px;
        font-size: 13px;
        border: 1px solid #ddd;
        border-radius: 3px;
    }

    input:focus {
        outline: none;
        border-color: #007BFF;
    }

    button {
        padding: 6px;
        font-size: 13px;
        background-color: #007BFF;
        color: white;
        border: none;
        border-radius: 3px;
        cursor: pointer;
    }

    button:hover {
        background-color: #0056b3;
    }
</style>