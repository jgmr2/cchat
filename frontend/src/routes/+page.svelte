<div>
    <SearchBar />
    {#if sessionToken && userEmail}
       <Profile on:toggleSidebar={toggleSidebar} />
    {:else}
        <LoginButton />
    {/if}

    {#if isSidebarOpen}
        <Sidebar email={userEmail} username={username} onClose={toggleSidebar} />
    {/if}
    <FileUpload />
</div>

<script>
    import SearchBar from "./SearchBar.svelte";
    import LoginButton from "./LoginButton.svelte";
    import Profile from "./Profile.svelte";
    import Sidebar from "./Sidebar.svelte";
    import FileUpload from "./FileUpload.svelte";

    let sessionToken = null;
    let userEmail = '';
    let username = '';
    let isSidebarOpen = false;

    // Verificar si estamos en el cliente
    if (typeof window !== 'undefined') {
        sessionToken = localStorage.getItem('sessionToken');

        // Decodificar el token para obtener el correo y el username del usuario
        if (sessionToken) {
            try {
                const payload = JSON.parse(atob(sessionToken.split('.')[1])); // Decodificar el payload del JWT
                userEmail = payload.email || ''; // Extraer el correo del payload
                username = payload.username || ''; // Extraer el username del payload
            } catch (err) {
                console.error('Error decoding token:', err);
                sessionToken = null; // Si hay un error, limpiar el token
            }
        }
    }

    // Función para alternar el estado del panel lateral
    function toggleSidebar() {
        isSidebarOpen = !isSidebarOpen;
    }
</script>