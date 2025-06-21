# 1 "/home/alexey/dev/pico/arduino-pico/uart/uart.ino"
# 2 "/home/alexey/dev/pico/arduino-pico/uart/uart.ino" 2

# 4 "/home/alexey/dev/pico/arduino-pico/uart/uart.ino" 2
# 5 "/home/alexey/dev/pico/arduino-pico/uart/uart.ino" 2
# 6 "/home/alexey/dev/pico/arduino-pico/uart/uart.ino" 2
# 7 "/home/alexey/dev/pico/arduino-pico/uart/uart.ino" 2
# 8 "/home/alexey/dev/pico/arduino-pico/uart/uart.ino" 2
# 23 "/home/alexey/dev/pico/arduino-pico/uart/uart.ino"
static volatile int uart_irq_count = 0;
static volatile int dma_irq_count = 0;

static int rx_dma_chan = -1;

// RX interrupt handler
void on_uart_rx() {
    uart_irq_count++;
    // Clear the IRQ by emptying the data
    while (uart_is_readable(((uart_inst_t *)((uart_hw_t *)0x40034000u)) /*|< Identifier for UART instance 0*/)) {
        uint8_t ch = uart_getc(((uart_inst_t *)((uart_hw_t *)0x40034000u)) /*|< Identifier for UART instance 0*/);
    }
}

// DMA interrupt handler
void on_dma_complete() {
    dma_irq_count++;
    // Clear the IRQ
    dma_channel_acknowledge_irq0(rx_dma_chan);
}

void setup() {
    delay(3000);
    Serial.begin(9600);
    Serial.println("Led turned on!");
}

void loop() {


    uint8_t tx_data[256];
    uint8_t rx_data[256];

    // Initialize stdio over USB
    // stdio_usb_init();

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(12, GPIO_FUNC_UART);
    gpio_set_function(13, GPIO_FUNC_UART);

    gpio_set_function(20, GPIO_FUNC_UART);
    gpio_set_function(21, GPIO_FUNC_UART);

    // Initialize our UARTs.
    uart_init(((uart_inst_t *)((uart_hw_t *)0x40034000u)) /*|< Identifier for UART instance 0*/, 115200);
    uart_init(((uart_inst_t *)((uart_hw_t *)0x40038000u)) /*|< Identifier for UART instance 1*/, 115200);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(((uart_inst_t *)((uart_hw_t *)0x40034000u)) /*|< Identifier for UART instance 0*/, false, false);
    uart_set_hw_flow(((uart_inst_t *)((uart_hw_t *)0x40038000u)) /*|< Identifier for UART instance 1*/, false, false);

    // Set our data format
    uart_set_format(((uart_inst_t *)((uart_hw_t *)0x40034000u)) /*|< Identifier for UART instance 0*/, 8, 1, UART_PARITY_NONE);
    uart_set_format(((uart_inst_t *)((uart_hw_t *)0x40038000u)) /*|< Identifier for UART instance 1*/, 8, 1, UART_PARITY_NONE);

    // Turn on the FIFOs
    uart_set_fifo_enabled(((uart_inst_t *)((uart_hw_t *)0x40034000u)) /*|< Identifier for UART instance 0*/, true);
    uart_set_fifo_enabled(((uart_inst_t *)((uart_hw_t *)0x40038000u)) /*|< Identifier for UART instance 1*/, true);

    // Set up the RX interrupt handler
    irq_set_exclusive_handler(20, on_uart_rx);

    // Set RX FIFO watermark to 16 bytes (half full)
    hw_write_masked(&uart_get_hw(((uart_inst_t *)((uart_hw_t *)0x40034000u)) /*|< Identifier for UART instance 0*/)->ifls, 2 << 3u, 0x00000038u);

    // Enable the UART to send RX timeout and FIFO watermark interrupts
    uart_get_hw(((uart_inst_t *)((uart_hw_t *)0x40034000u)) /*|< Identifier for UART instance 0*/)->imsc = 0x00000010u | 0x00000040u;

    // Enable the RX interrupt handler
    irq_set_enabled(20, true);

    // Get the DMA channel for RX, panic() if there are none
    rx_dma_chan = dma_claim_unused_channel(true);

    dma_channel_config rx_dma_config = dma_channel_get_default_config(rx_dma_chan);
    channel_config_set_read_increment(&rx_dma_config, false);
    channel_config_set_write_increment(&rx_dma_config, true);
    channel_config_set_dreq(&rx_dma_config, uart_get_dreq(((uart_inst_t *)((uart_hw_t *)0x40034000u)) /*|< Identifier for UART instance 0*/, false));
    channel_config_set_transfer_data_size(&rx_dma_config, DMA_SIZE_8);
    // channel_config_set_ring(&rx_dma_config, true, 8);

    dma_channel_set_config(rx_dma_chan, &rx_dma_config, false);
    dma_channel_set_read_addr(rx_dma_chan, &uart_get_hw(((uart_inst_t *)((uart_hw_t *)0x40034000u)) /*|< Identifier for UART instance 0*/)->dr, false);
    // dma_channel_set_write_addr(rx_dma_chan, rx_data, false);

    // Set up the DMA interrupt handler
    irq_set_exclusive_handler(11, on_dma_complete);
    dma_channel_set_irq0_enabled(rx_dma_chan, true);

    // Enable the DMA interrupt handler
    irq_set_enabled(11, true);

    // OK, all set up.
    // Lets wait for USB stdio to be active, and then run the test loop and report results
    for (int i = 0; i < 256; i++) {
        tx_data[i] = i & 0xFF;
        rx_data[i] = 0;
    }

    //while (!stdio_usb_connected())
    //    tight_loop_contents();

    // Turn on the DMA transfer
    dma_channel_transfer_to_buffer_now(rx_dma_chan, rx_data, 256);

    // Send the data
    uart_write_blocking(((uart_inst_t *)((uart_hw_t *)0x40038000u)) /*|< Identifier for UART instance 1*/, tx_data, 256);

    // Send some more incomplete data
    uart_write_blocking(((uart_inst_t *)((uart_hw_t *)0x40038000u)) /*|< Identifier for UART instance 1*/, tx_data, 23);

    // Wait for everything to be finished
    sleep_ms(100);

    // Report the activity
    printf("%d calls to UART IRQ\n", uart_irq_count);
    printf("%d calls to DMA IRQ\n", dma_irq_count);

    printf("\nRX buffer contains:\n");
    for (int i = 0; i < 256; i += 16) {
        for (int j = 0; j < 16; j++) {
            printf("%02X ", rx_data[i + j]);
        }
        printf("\n");
    }

    while (1)
        tight_loop_contents();
}
