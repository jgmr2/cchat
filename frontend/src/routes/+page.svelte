<div>
 
    <SearchBar bind:searchResults={searchResults} bind:isSearching={isSearching} />

    <!-- Mostrar la galería solo si hay resultados -->
    {#if isSearching}
        <Gallery {searchResults} />
    {/if}

    {#if sessionToken && userEmail}
        <Profile on:toggleSidebar={toggleSidebar} />
        <FloatingButton on:toggleFileUploadPanel={toggleFileUploadPanel} />
    {:else}
        <LoginButton />
    {/if}

    {#if isSidebarOpen}
        <Sidebar email={userEmail} username={username} onClose={toggleSidebar} />
    {/if}

    {#if isFileUploadOpen}
        <div class="file-upload-panel">
            <FileUpload onClose={toggleFileUploadPanel} />
        </div>
    {/if}
</div>

<script>
    import SearchBar from "./components/SearchBar.svelte";
    import Gallery from "./components/Gallery.svelte";
    import LoginButton from "./components/LoginButton.svelte";
    import Profile from "./components/Profile.svelte";
    import Sidebar from "./components/Sidebar.svelte";
    import FileUpload from "./components/FileUpload.svelte";
    import FloatingButton from "./components/FloatingButton.svelte";

    
    let searchResults = []; // Variable para almacenar los resultados de búsqueda
    let isSearching = false; // Variable para controlar el estado de búsqueda
    let sessionToken = null;
    let userEmail = '';
    let username = '';
    let isSidebarOpen = false;
    let isFileUploadOpen = false; // Estado para el panel de FileUpload

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

    // Función para alternar el estado del panel de FileUpload
    function toggleFileUploadPanel() {
        isFileUploadOpen = !isFileUploadOpen;
    }
</script>

<style>
    .file-upload-panel {
        position: fixed;
        top: 0;
        right: 0;
        width: 300px;
        height: 100%;
        background-color: white;
        box-shadow: -2px 0 5px rgba(0, 0, 0, 0.1);
        z-index: 1000;
        display: flex;
        flex-direction: column;
        padding: 1rem;
    }
</style>