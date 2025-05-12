<form class="login-form" on:submit|preventDefault={handleLogin}>
    <label for="email">Email :</label>
    <input type="email" id="email" name="email" bind:value={email} placeholder="Email" />

    <label for="password">Mot de passe :</label>
    <input type="password" id="password" name="password" bind:value={password} placeholder="Mot de passe" />

    <button type="submit">Connexion</button>
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
                body: JSON.stringify({ email, password }) // Envoyer l'email et le mot de passe
            });

            if (response.ok) {
                const data = await response.json();
                console.log('Connexion réussie :', data);

                // Stocker le token dans le localStorage
                if (data.token) {
                    localStorage.setItem('sessionToken', data.token);
                }

                // Rediriger l'utilisateur après une connexion réussie
                goto('/'); // Remplacez '/' par la route souhaitée
            } else {
                const error = await response.json();
                console.error('Échec de la connexion :', error.message);
                alert('Échec de la connexion : ' + error.message);
            }
        } catch (err) {
            console.error('Erreur lors de la connexion à l\'API :', err);
            alert('Une erreur est survenue. Veuillez réessayer.');
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