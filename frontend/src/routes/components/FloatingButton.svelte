<script>
    import { createEventDispatcher } from 'svelte';

    const dispatch = createEventDispatcher();

    function handleClick() {
        dispatch('toggleFileUploadPanel'); // Emitir el evento correcto al componente padre
    }
</script>

<button class="floating-button" on:click={handleClick}>
    +
</button>

<style>
    .floating-button {
        position: fixed;
        bottom: 20px;
        right: 20px;
        width: 50px;
        height: 50px;
        background-color: #007BFF;
        color: white;
        border: none;
        border-radius: 50%;
        font-size: 24px;
        font-weight: bold;
        cursor: pointer;
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        transition: background-color 0.3s, transform 0.2s;
        z-index: 1000;
    }

    .floating-button:hover {
        background-color: #0056b3;
    }

    .floating-button:active {
        transform: scale(0.95);
    }
</style>