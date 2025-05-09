<template>
    <div class="login-page">
        <div class="login-form">
            <h1>Login</h1>
            <form @submit.prevent="handleLogin">
                <div>
                    <label for="email">Email:</label>
                    <input type="email" id="email" v-model="email" required />
                </div>
                <div>
                    <label for="password">Password:</label>
                    <input type="password" id="password" v-model="password" required />
                </div>
                <button type="submit">Login</button>
            </form>
            <p v-if="errorMessage" class="error">{{ errorMessage }}</p>
        </div>
    </div>
</template>

<script>
export default {
    data() {
        return {
            email: "", // Cambiado de username a email
            password: "",
            errorMessage: "",
        };
    },
    methods: {
        async handleLogin() {
            try {
                // Enviar solicitud POST al endpoint de login usando $fetch
                const response = await $fetch("http://localhost:81/auth/login", {
                    method: "POST",
                    body: {
                        email: this.email, // Usar email en lugar de username
                        password: this.password,
                    },
                });

                // Manejar la respuesta exitosa
                const token = response.token;
                localStorage.setItem("authToken", token); // Guardar el token en localStorage
                alert("Login successful!");
                this.$router.push("/"); // Redirigir al usuario a otra página
            } catch (error) {
                // Manejar errores
                if (error.data && error.data.error) {
                    this.errorMessage = error.data.error;
                } else {
                    this.errorMessage = "An error occurred. Please try again.";
                }
            }
        },
    },
};
</script>

<style scoped>
.login-page {
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
    background-color: #f5f5f5;
}

.login-form {
    background: white;
    padding: 20px;
    border-radius: 8px;
    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
    width: 300px;
}

.error {
    color: red;
    margin-top: 10px;
}
</style>