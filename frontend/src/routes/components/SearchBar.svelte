<script>
    export let placeholder = "Buscar...";
    export let value = "";
    export let onSearch = () => {};

    let isSearching = false;

    function handleInput(event) {
        value = event.target.value;
        onSearch(value);
        if (!isSearching) {
            isSearching = true;
        }
    }

    function handleKeydown(event) {
        if (event.key === "Escape") {
            isSearching = false;
            value = "";
        }
    }

    
</script>

<div
    class="search-container {isSearching ? 'searching' : ''}"
    on:keydown={handleKeydown}
    tabindex="0"
>
    <div class="logo-and-bar">
        <img
            src="/logo.png"
            alt="Logo"
            class="logo {isSearching ? 'logo-small' : ''}"
        />
        <div class="relative search-bar">
            <input
                type="text"
                bind:value={value}
                on:input={handleInput}
                placeholder={placeholder}
                class="w-full px-6 py-4 text-lg text-gray-700 bg-gray-100 border border-gray-300 rounded-full shadow-lg focus:outline-none focus:ring-4 focus:ring-blue-500 focus:border-blue-500"
            />
        </div>
    </div>
</div>

<style>
    .search-container {
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
        height: 90vh;
        width: 60vw;
        transition: all 0.1s ease-in-out;
        position: relative;
        margin: auto;
    }

    .search-container.searching {
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: auto;
        background-color: rgba(255, 255, 255, 0);
        z-index: 10;
        padding: 10px 20px;
        justify-content: flex-start;
        align-items: flex-start;
    }

    .logo-and-bar {
        display: flex;
        flex-direction: column;
        align-items: center;
        transition: all 0.1s ease-in-out;
    }

    .search-container.searching .logo-and-bar {
        flex-direction: row;
        align-items: center;
        width: 100%;
    }

    .logo {
        width: 150px;
        height: auto;
        transition: all 0.1s ease-in-out;
        margin-bottom: 20px;
    }

    .search-container.searching .logo {
        width: 80px;
        margin-bottom: 0;
        margin-right: 20px;
    }

    .search-bar {
        width: 120%; /* Aumenta el ancho del contenedor en un 20% */
        transition: all 0.1s ease-in-out;
    }

    input {
        width: 100%; /* El input ocupa todo el ancho del contenedor */
        font-size: 1.5rem;
        padding: 1rem 2rem;
        border-radius: 3.1416px;
        border: none; /* Elimina los bordes */
        outline: none; /* Elimina el borde azul de Firefox */
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        color: rgba(12, 78, 77, 0.715);
        background-color: rgba(5, 88, 96, 0.097);
    }

    .search-container.searching input {
        padding: 0.7rem 1.0rem; /* Reduce el padding en un 30% */
        font-size: 1.4rem;
        width: 40%;
    }

    input:focus {
        background-color: #ffffff;
        box-shadow: 0 6px 10px rgba(0, 0, 0, 0.15);
    }
</style>