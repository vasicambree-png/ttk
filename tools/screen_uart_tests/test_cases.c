static unsigned checks, failures;
#define CHECK(c) do { checks++; if (!(c)) { failures++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

static uint16_t make_page(uint8_t *out, uint8_t rank, uint8_t menu, uint8_t focus,
                          const uint16_t *params, uint8_t count)
{
    uint16_t len = (uint16_t)(6u + 2u * count), i;
    out[0] = 0xaa; out[1] = 0xee; out[2] = (uint8_t)(len >> 8);
    out[3] = (uint8_t)len; out[4] = 1; out[5] = 1;
    out[6] = rank; out[7] = menu; out[8] = focus; out[9] = count;
    for (i = 0; i < count; i++) {
        out[10 + i * 2] = (uint8_t)(params[i] >> 8);
        out[11 + i * 2] = (uint8_t)params[i];
    }
    out[10 + count * 2] = calculate_checksum(out + 4, len);
    out[11 + count * 2] = 0x0a;
    return (uint16_t)(len + 6);
}

static void enqueue(const uint8_t *frame, uint16_t len)
{
    if (queue_head == queue_tail) queue_head = queue_tail = 0;
    CHECK(queue_tail < 8);
    memcpy(queued[queue_tail], frame, len);
    queued_len[queue_tail++] = len;
}

static void pump(unsigned ticks)
{
    unsigned i;
    for (i = 0; i < ticks; i++) usart_ProcessEvent(0, START_IO_EVT);
    usart_ProcessEvent(0, START_DATA_EVT);
}

static void reset(void)
{
    memset(&Data_list1, 0, sizeof(Data_list1));
    memset(&screen_power, 0, sizeof(screen_power));
    memset(&last_drawn, 0, sizeof(last_drawn));
    mock_clock = 0; draws = off_calls = on_calls = drains = round_ticks = report_calls = 0;
    cmd03_requests = cmd03_ticks = cmd03_answers = response_calls = 0;
    init_calls = saves = clears = ble_ticks = name_ticks = 0;
    mock_round_complete = mock_cmd03_ready = 0;
    g_scan_mode = g_binding_count = g_store_dirty = 0;
    Rx_sleep_flag = 1; dis_flag_cnt = 0; g_frame_last_sec = FRAME_AGE_NEVER;
    ui_refresh_tick = ui_tick_10ms = ui_last_redraw_10ms = 0;
    g_ui_reinit_req = g_store_clear_req = 0;
    fifo_len_flag = 0; fifo_time_Cnt = 0; queue_head = queue_tail = 0;
}

int main(void)
{
    uint8_t frame[128], bad[128];
    uint16_t settings[] = { 2, 10, 1, 1, 0 };
    uint16_t home[] = { 101, 3200, 3, 4, 5, 6, 77, 1, 1, 0, 1234 };
    uint16_t len;
    unsigned old_draws, old_drains, i;
    data_LIST saved;

    reset();
    len = make_page(frame, 3, 5, 1, settings, 5);
    enqueue(frame, len); app_uart_process(); pump(5);
    CHECK(Data_list1.Menu_rank6.time_light == 10);
    CHECK(on_calls == 1 && Rx_sleep_flag == 0 && draws == 1);

    mock_clock = 15999;
    settings[0] = 3; /* live setting value unrelated to timeout/navigation */
    len = make_page(frame, 3, 5, 1, settings, 5);
    enqueue(frame, len); app_uart_process(); pump(5);
    CHECK(off_calls == 0 && Rx_sleep_flag == 0);
    mock_clock = 16000; pump(5);
    CHECK(off_calls == 1 && Rx_sleep_flag == 1);
    old_draws = draws; old_drains = drains;
    for (i = 0; i < 3; i++) {
        settings[0] = (uint16_t)(4 + i);
        len = make_page(frame, 3, 5, 1, settings, 5);
        enqueue(frame, len); app_uart_process(); pump(20);
    }
    CHECK(Data_list1.Menu_rank6.power == 6);
    CHECK(draws == old_draws && on_calls == 1 && off_calls == 1);
    CHECK(drains == old_drains + 60);
    puts("PASS: 10-second boundary, power-save transition, settings cache and BLE drain while off");

    /* Main page values update while off; no screen wake or timeout refresh. */
    settings[0] = 2;
    mock_clock = 17000;
    len = make_page(frame, 1, 0, 0, home, 11);
    enqueue(frame, len); app_uart_process(); pump(5);
    CHECK(Rx_sleep_flag == 0 && on_calls == 2);
    mock_clock = 33000; pump(5);
    CHECK(Rx_sleep_flag == 1 && off_calls == 2);
    old_draws = draws;
    for (i = 0; i < 4; i++) {
        home[1] = (uint16_t)(3300 + i); home[10] = (uint16_t)(2000 + i);
        len = make_page(frame, 1, 0, 0, home, 11);
        enqueue(frame, len);
    }
    app_uart_process(); pump(5);
    CHECK(queue_head == queue_tail);
    CHECK(Data_list1.UI_main.vbat == 3303 && Data_list1.UI_main.data[0] == 2003);
    CHECK(Rx_sleep_flag == 1 && draws == old_draws && on_calls == 2);
    puts("PASS: actual app_uart_process drains four frames and stores latest home data while off");

    { uint8_t request[] = { 0xaa, 0xee, 0, 1, 3, 3, 0x0a };
      enqueue(request, sizeof(request)); app_uart_process(); }
    CHECK(cmd03_requests == 1);
    g_binding_count = 1; mock_round_complete = mock_cmd03_ready = 1; g_store_dirty = 1;
    usart_ProcessEvent(0, START_TIMER_EVT);
    CHECK(cmd03_ticks == 1 && cmd03_answers == 1 && report_calls == 1);
    CHECK(round_ticks == 1 && ble_ticks == 1 && name_ticks == 1 && saves == 1);
    CHECK(Rx_sleep_flag == 1);
    puts("PASS: 0x03 request/answer, 0x06 report, Flash and BLE/name heartbeat continue while off");

    home[10] = 9999;
    len = make_page(frame, 1, 0, 1, home, 11);
    enqueue(frame, len); app_uart_process(); pump(5);
    CHECK(Rx_sleep_flag == 0 && on_calls == 3 && draws == old_draws + 1);
    CHECK(last_drawn.UI_main.vbat == 3303 && last_drawn.UI_main.data[0] == 9999);
    puts("PASS: menu focus change wakes once and redraws latest cached data");

    saved = Data_list1;
    mock_clock = 49000;
    memcpy(bad, frame, len); bad[len - 2] ^= 0xff;
    CHECK(parse_received_frame(bad, len, &Data_list1) != 0);
    CHECK(memcmp(&saved, &Data_list1, sizeof(saved)) == 0);
    len = make_page(bad, 1, 0, 2, home, 9); /* checksum-valid but incomplete home */
    CHECK(parse_received_frame(bad, len, &Data_list1) != 0);
    CHECK(memcmp(&saved, &Data_list1, sizeof(saved)) == 0);
    CHECK(parse_received_frame(NULL, 0, &Data_list1) != 0);
    pump(5);
    CHECK(Rx_sleep_flag == 1 && off_calls == 3 && on_calls == 3);
    puts("PASS: malformed/short frames do not mutate cache, wake display or restart timeout");

    /* A full clock cycle cannot silently reopen the display after timeout. */
    mock_clock = 33000;
    len = make_page(frame, 1, 0, 1, home, 11);
    enqueue(frame, len); app_uart_process(); pump(5);
    CHECK(Rx_sleep_flag == 1 && on_calls == 3 && off_calls == 3);
    puts("PASS: expired display stays off after clock wraps with ordinary refresh");

    mock_clock = 50000; settings[1] = 0;
    len = make_page(frame, 3, 5, 1, settings, 5);
    enqueue(frame, len); app_uart_process(); pump(5);
    mock_clock = 1000000; pump(5);
    CHECK(Rx_sleep_flag == 0);
    puts("PASS: time_light=0 keeps display on");

    reset(); settings[1] = 10;
    len = make_page(frame, 3, 5, 1, settings, 5);
    enqueue(frame, len); app_uart_process(); pump(5);
    mock_clock = 16000; pump(5);
    saved = Data_list1;
    for (i = 3; i <= 4; i++) {
        uint32_t activity = screen_power.last_activity;
        mock_clock += 100;
        len = make_page(frame, 3, 5, 2, settings, (uint8_t)i);
        CHECK(parse_received_frame(frame, len, &Data_list1) != 0);
        CHECK(memcmp(&saved, &Data_list1, sizeof(saved)) == 0);
        CHECK(screen_power.last_activity == activity);
        pump(5);
        CHECK(Rx_sleep_flag == 1 && on_calls == 1 && off_calls == 1);
    }
    puts("PASS: checksum-valid settings missing one/both metadata fields cannot mutate or wake");

    mock_clock = 17000;
    len = make_page(frame, 5, 0, 0, NULL, 0);
    enqueue(frame, len); app_uart_process();
    CHECK(g_ui_reinit_req == 1 && init_calls == 0);
    pump(5);
    CHECK(Rx_sleep_flag == 0 && on_calls == 2);
    usart_ProcessEvent(0, START_TIMER_EVT); pump(5);
    CHECK(init_calls == 1 && g_ui_reinit_req == 0 && Rx_sleep_flag == 0);
    puts("PASS: explicit rank5 reinitialization wakes and runs in deferred timer event");

    saved = Data_list1;
    { uint32_t activity = screen_power.last_activity;
      uint16_t message[] = { UI_MSG_PARAM_SAVE_OK };
      mock_clock = 32999;
      len = make_page(frame, 6, 5, 2, message, 1);
      enqueue(frame, len); app_uart_process();
      CHECK(screen_power.last_activity == activity);
      CHECK(memcmp(&saved, &Data_list1, sizeof(saved)) == 0); }
    mock_clock = 33000; pump(5);
    CHECK(Rx_sleep_flag == 1 && off_calls == 2 && on_calls == 2);
    puts("PASS: rank6 message keeps cached page and does not extend screen timeout");

    mock_clock = 34000;
    len = make_page(frame, 5, 0, 0, NULL, 0);
    enqueue(frame, len); app_uart_process();
    CHECK(g_ui_reinit_req == 1);
    /* Hold the deferred initializer until the requested display time has expired. */
    mock_clock = 50000;
    usart_ProcessEvent(0, START_TIMER_EVT); pump(5);
    CHECK(init_calls == 1 && g_ui_reinit_req == 0);
    CHECK(Rx_sleep_flag == 1 && off_calls == 2 && on_calls == 2);
    puts("PASS: expired deferred rank5 request cannot reinitialize or reopen display");

    /* Return-home is menu 7, with only the two shared metadata words. */
    reset();
    {
        uint16_t metadata[] = { 2, 5 };
        screen_power_t policy;
        uint8_t selected;
        len = make_page(frame, 2, 7, 0, metadata, 2);
        CHECK(len == 16);
        enqueue(frame, len); app_uart_process(); pump(5);
        CHECK(Data_list1.menu_rank == 2 && Data_list1.rank2_addr == 7 && UI_Select == 7);
        CHECK(Data_list1.UI_main.chu_num2 == 2 && Data_list1.UI_main.re_flag == 5);
        CHECK(last_drawn.menu_rank == 2 && last_drawn.rank2_addr == 7 && draws == 1);
        CHECK(screen_power.seen && screen_power.menu == 7 && g_frame_last_sec == 0);

        metadata[0] = 1; metadata[1] = 4;
        len = make_page(frame, 3, 7, 1, metadata, 2);
        enqueue(frame, len); app_uart_process(); pump(5);
        CHECK(last_drawn.menu_rank == 3 && last_drawn.rank2_addr == 7 && last_drawn.rank3_addr == 1);
        CHECK(last_drawn.UI_main.chu_num2 == 1 && last_drawn.UI_main.re_flag == 4);
        CHECK(draws == 2 && UI_Select == 7);

        saved = Data_list1; policy = screen_power; selected = UI_Select;
        g_frame_last_sec = 9;
        mock_clock += 100;
        for (i = 0; i < 2; ++i) {
            len = make_page(bad, 3, 7, 0, metadata, (uint8_t)i);
            CHECK(parse_received_frame(bad, len, &Data_list1) != 0);
            CHECK(memcmp(&saved, &Data_list1, sizeof(saved)) == 0);
            CHECK(memcmp(&policy, &screen_power, sizeof(policy)) == 0);
            CHECK(UI_Select == selected && g_frame_last_sec == 9);
        }
        len = make_page(bad, 3, 8, 0, metadata, 2);
        CHECK(parse_received_frame(bad, len, &Data_list1) != 0);
        CHECK(memcmp(&saved, &Data_list1, sizeof(saved)) == 0);
        CHECK(memcmp(&policy, &screen_power, sizeof(policy)) == 0);
        CHECK(UI_Select == selected && g_frame_last_sec == 9);
        len = make_page(bad, 3, 7, 0, metadata, 2);
        CHECK(parse_received_frame(bad, (uint16_t)(len - 1), &Data_list1) != 0);
        bad[len - 2] ^= 0xff;
        CHECK(parse_received_frame(bad, len, &Data_list1) != 0);
        CHECK(memcmp(&saved, &Data_list1, sizeof(saved)) == 0);
        CHECK(memcmp(&policy, &screen_power, sizeof(policy)) == 0);
        CHECK(UI_Select == selected && g_frame_last_sec == 9);

        /* The controller completes confirmation with its normal home frame. */
        len = make_page(frame, 1, 0, 0, home, 11);
        enqueue(frame, len); app_uart_process(); pump(5);
        CHECK(last_drawn.menu_rank == 1 && Data_list1.menu_rank == 1 && draws == 3);
        len = make_page(frame, 2, 7, 0, metadata, 2);
        enqueue(frame, len); app_uart_process(); pump(5);
        CHECK(last_drawn.menu_rank == 2 && last_drawn.rank2_addr == 7 && draws == 4);
    }
    puts("PASS: return-home menu 7 accepts metadata, rejects incomplete/unknown frames and reenters after home");
    printf("%u checks, %u failures\n", checks, failures);
    return failures ? 1 : 0;
}
