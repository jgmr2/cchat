<form class="register-form" on:submit|preventDefault={handleRegister}>
    <label for="username">Username:</label>
    <input type="text" id="username" bind:value={username} name="username" placeholder="Enter your username" />

    <label for="email">Email:</label>
    <input type="email" id="email" bind:value={email} name="email" placeholder="Enter your email" />

    <label for="password">Password:</label>
    <input type="password" id="password" bind:value={password} name="password" placeholder="Enter your password" />

    <label for="confirm-password">Confirm Password:</label>
    <input type="password" id="confirm-password" bind:value={confirmPassword} name="confirm-password" placeholder="Confirm your password" />

    <button type="submit">Register</button>

    {#if errorMessage}
        <p class="error-message">{errorMessage}</p>
    {/if}

    {#if successMessage}
        <p class="success-message">{successMessage}</p>
    {/if}
</form>

<script>
    let username = '';
    let email = '';
    let password = '';
    let confirmPassword = '';
    let errorMessage = '';
    let successMessage = '';

    async function handleRegister() {
        // Validar que las contraseñas coincidan
        if (password !== confirmPassword) {
            errorMessage = "Passwords do not match.";
            successMessage = '';
            return;
        }

        try {
            // Enviar solicitud POST al endpoint
            const response = await fetch('http://localhost:81/auth/register', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    username,
                    email,
                    password
                })
            });

            if (!response.ok) {
                const errorData = await response.json();
                errorMessage = errorData.error || "Failed to register.";
                successMessage = '';
                return;
            }

            // Mostrar mensaje de éxito
            const data = await response.json();
            successMessage = data.message || "Registration successful!";
            errorMessage = '';

            // Limpiar los campos del formulario
            username = '';
            email = '';
            password = '';
            confirmPassword = '';
        } catch (error) {
            errorMessage = "An error occurred. Please try again.";
            successMessage = '';
            console.error(error);
        }
    }
</script>

<style>
    .register-form {
        display: flex;
        flex-direction: column;
        gap: 8px;
    }

    label {
        font-size: 14px;
        color: #333;
    }

    input {
        padding: 8px;
        font-size: 14px;
        border: 1px solid #ccc;
        border-radius: 4px;
    }

    input:focus {
        outline: none;
        border-color: #28a745;
        box-shadow: 0 0 4px rgba(40, 167, 69, 0.5);
    }

    button {
        padding: 8px;
        font-size: 14px;
        background-color: #28a745;
        color: white;
        border: none;
        border-radius: 4px;
        cursor: pointer;
        transition: background-color 0.3s;
    }

    button:hover {
        background-color: #218838;
    }

    .error-message {
        color: #ff4d4d;
        font-size: 14px;
    }

    .success-message {
        color: #28a745;
        font-size: 14px;
    }
</style>