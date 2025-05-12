<form class="register-form" on:submit|preventDefault={handleRegister}>
    <label for="username">Nom d'utilisateur :</label>
    <input type="text" id="username" bind:value={username} name="username" placeholder="Entrez votre nom d'utilisateur" />

    <label for="email">Email :</label>
    <input type="email" id="email" bind:value={email} name="email" placeholder="Entrez votre email" />

    <label for="password">Mot de passe :</label>
    <input type="password" id="password" bind:value={password} name="password" placeholder="Entrez votre mot de passe" />

    <label for="confirm-password">Confirmez le mot de passe :</label>
    <input type="password" id="confirm-password" bind:value={confirmPassword} name="confirm-password" placeholder="Confirmez votre mot de passe" />

    <button type="submit">S'inscrire</button>

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
        // Vérifier que les mots de passe correspondent
        if (password !== confirmPassword) {
            errorMessage = "Les mots de passe ne correspondent pas.";
            successMessage = '';
            return;
        }

        try {
            // Envoyer une requête POST au point de terminaison
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
                errorMessage = errorData.error || "Échec de l'inscription.";
                successMessage = '';
                return;
            }

            // Afficher un message de succès
            const data = await response.json();
            successMessage = data.message || "Inscription réussie !";
            errorMessage = '';

            // Réinitialiser les champs du formulaire
            username = '';
            email = '';
            password = '';
            confirmPassword = '';
        } catch (error) {
            errorMessage = "Une erreur est survenue. Veuillez réessayer.";
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