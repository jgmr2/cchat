<div class="sidebar">
    <button class="close-btn" on:click={onClose}>X</button>
    <h2>User Information</h2>
    <p><strong>Email:</strong> {email}</p>
    <p><strong>Username:</strong> {username}</p>
    <button class="logout-btn" on:click={handleLogout}>Logout</button>
</div>

<script>
    export let email = '';
    export let username = '';
    export let onClose = () => {};

    function handleLogout() {
        // Eliminar el token de sesión del localStorage
        localStorage.removeItem('sessionToken');

        // Recargar la página o redirigir al usuario a la página de inicio de sesión
        window.location.reload();
    }
</script>

<style>
    .sidebar {
        position: fixed;
        top: 0;
        right: 0;
        width: 300px;
        height: 100%;
        background-color: #f4f4f4;
        box-shadow: -2px 0 5px rgba(0, 0, 0, 0.2);
        padding: 20px;
        z-index: 1000;
    }

    .close-btn {
        background: none;
        border: none;
        font-size: 1.5rem;
        cursor: pointer;
        position: absolute;
        top: 10px;
        right: 10px;
    }

    .logout-btn {
        margin-top: 20px;
        padding: 10px 20px;
        background-color: #ff4d4d;
        color: white;
        border: none;
        border-radius: 4px;
        cursor: pointer;
        font-size: 14px;
        transition: background-color 0.3s;
    }

    .logout-btn:hover {
        background-color: #e60000;
    }
</style>