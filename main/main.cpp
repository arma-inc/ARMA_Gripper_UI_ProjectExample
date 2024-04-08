// ARMA Gripper UI - Main File Example

/*
  Software License Agreement (BSD-1-Clause)

  Copyright (c) 2024 ARMA Inc. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <M5Unified.h>

#include "sdkconfig.h"
#ifdef CONFIG_ARMA_UI_M5Core2

#define MB_UART_TXD 14
#define MB_UART_RXD 13

#elif CONFIG_ARMA_UI_M5CoreS3

#define MB_UART_TXD 17
#define MB_UART_RXD 18

#else
#error "Device is not supported!"
#endif

#include "esp_log.h"
#include "esp_modbus_master.h"

#define MB_TIMEOUT_MS (500)
#define MB_TIMEOUT_TICS (MB_TIMEOUT_MS / portTICK_PERIOD_MS)
#define TAG "UI"

#include "arma_m5_init.h"  // DO NOT DELETE!
#include "arma_ui.h"

enum class MB_STATE {
    INIT,
    INIT_OK,
    OFFLINE,
    ONLINE,
};
static MB_STATE mb_state = MB_STATE::INIT;

#define INVALID 65535
#define IRA_DI 0x0000
#define IRA_ERROR_STATUS 0x0001
#define IRA_CURRENT_POSITION 0x0002
#define IR_SIZE 3
uint16_t IR[IR_SIZE] = { 0, INVALID, INVALID };
bool IR_CHANGE[IR_SIZE] = { true, true, true };

#define HRA_POS0 0x0000
#define HRA_POS1 0x0001
#define HRA_TARGET_POS 0x0002
#define HR_SIZE 24
uint16_t HR[HR_SIZE];
bool HR_CHANGE[HR_SIZE] = { 0 };

#define CA_STOP 0x0000
#define CA_CONTROL_MODE 0x0001
#define CA_SAVE_CONFIG 0x0002
#define CA_START_CALIBRATION 0x0003
#define CA_REBOOT 0x0004
#define CA_OUTPUT_ACTIVE_LEVEL 0x0005
uint16_t COILS = 0;
uint16_t COILS_CHANGE = 0xFFFF;

#define DIA_IN_POS0 0x0000
#define DIA_IN_POS1 0x0001
#define DIA_COLLISION 0x0002
#define DIA_STOPPED 0x0003
#define DIA_CALIBRATION_IN_PROGRESS 0x0004
#define DIA_IN0 0x0005
#define DIA_IN1 0x0006
#define DIA_IN2 0x0007
#define DIA_IN3 0x0008
#define DIA_OUT0H 0x0009
#define DIA_OUT0L 0x000A
#define DIA_OUT1H 0x000B
#define DIA_OUT1L 0x000C
#define DIA_WARN_MDT 0x000D

// Map register value from range (0 ~ 65534) to range (0 ~ 100)
#define RV100(RV) (((uint32_t)(RV) * 100u + 32767u) / 65534u)

enum {
    B_WIDTH = 320 / 3 - 2,
    B_HEIGHT = 42,

    BX1 = 0,
    BX2 = BX1 + B_WIDTH + 4,
    BX3 = BX2 + B_WIDTH + 4,

    BTY1 = 0,
    BTY2 = BTY1 + B_HEIGHT + 4,

    BBY1 = 240 - B_HEIGHT - 4,
    BBY2 = BBY1 - B_HEIGHT - 4,

    TY_TITLE = BTY2 + 21,
    TY_COILS = TY_TITLE + 24,
    TY_STATUS = TY_COILS + 24,
    TY_POS = TY_STATUS + 24,
    TY_AG_NC = 200,

};
static UIButton btn_pos0(BX1, BBY1, B_WIDTH, B_HEIGHT, "Pos0");
static UIButton btn_pos1(BX3, BBY1, B_WIDTH, B_HEIGHT, "Pos1");
static UIButton btn_cm(BX2, BBY1, B_WIDTH, B_HEIGHT, "Control");

static UIButton btn_stop(BX1, BTY1, B_WIDTH, B_HEIGHT, "STOP");
static UIButton btn_reboot(BX2, BTY1, B_WIDTH, B_HEIGHT, "REBOOT");
static UIButton btn_cal(BX3, BTY1, B_WIDTH, B_HEIGHT, "CAL");

typedef void (*btn_action_t)(void);
struct ButtonAndAction {
    UIButton* btn;
    btn_action_t action;
};

static void ag_pos0();
static void ag_pos1();
static void ag_cm();
static void ag_stop();
static void ag_reboot();
static void ag_cal();

static ButtonAndAction const BUTTONS[] = {
    { &btn_pos0, ag_pos0 }, { &btn_pos1, ag_pos1 },     { &btn_cm, ag_cm },
    { &btn_stop, ag_stop }, { &btn_reboot, ag_reboot }, { &btn_cal, ag_cal },
};
static const size_t BUTTONS_SIZE = sizeof(BUTTONS) / sizeof(BUTTONS[0]);

static void update_registers(uint16_t* regs, bool* changes, size_t size);
static void update_ui(bool force_draw = false);
static void update_ui_error_status();
static void update_ui_position();
static void update_ui_coils();
static void update_ui_status();
static void print_arma_gripper();

void setup() {
    // This function must be executed initially to prevent damage to the M5.
    // It initializes the M5 and configures its power chip.
    arma_m5_init();  // DO NOT DELETE!

    LCD.setBrightness(63);
    LCD.fillScreen(BLACK);
    print_arma_gripper();

    vTaskDelay(pdMS_TO_TICKS((2000)));

#ifdef CONFIG_ARMA_UI_M5Core2
    printf("\n\nARMA Gripper UI Example - M5 Core2\n");
#elif CONFIG_ARMA_UI_M5CoreS3
    printf("\n\nARMA Gripper UI Example - M5 CoreS3\n");
#endif

    // Initialize and start Modbus controller
    mb_communication_info_t comm = {
        .mode = MB_MODE_RTU,
        .slave_addr = 0,  // master
        .port = UART_NUM_2,
        .baudrate = 115200,
        .parity = UART_PARITY_EVEN,
        .dummy_port = 0,
    };
    void* master_handler = NULL;

    esp_err_t err = mbc_master_init(MB_PORT_SERIAL_MASTER, &master_handler);
    if ((master_handler == NULL) || (err != ESP_OK)) {
        esp_log_write(
            ESP_LOG_ERROR, TAG, "mb controller initialization fail, returns(0x%x).", (int)err);
        return;
    }

    err = mbc_master_setup((void*)&comm);
    if ((err != ESP_OK)) {
        esp_log_write(ESP_LOG_ERROR, TAG, "mb controller setup fail, returns(0x%x).", (int)err);
        return;
    }

    // Set UART pin numbers
    err =
        uart_set_pin(comm.port, MB_UART_TXD, MB_UART_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if ((err != ESP_OK)) {
        esp_log_write(ESP_LOG_ERROR, TAG, "uart pin setup fail, returns(0x%x).", (int)err);
        return;
    }

    err = mbc_master_start();
    if (err != ESP_OK) { ESP_LOGE(TAG, "mb controller start fail, err=%x.", err); }
    mb_state = MB_STATE::INIT_OK;
}

static uint16_t mb_res[256] = { 0 };

static mb_param_request_t s0x41_read_c = {
    .slave_addr = 0x41,
    .command = 0x01,  // MB_FUNC_READ_COILS
    .reg_start = 0x0000,
    .reg_size = 16,
};

static mb_param_request_t s0x41_read_hr = {
    .slave_addr = 0x41,
    .command = 0x03,  // MB_FUNC_READ_HOLDING_REGISTER
    .reg_start = 0x0000,
    .reg_size = HR_SIZE,
};

static mb_param_request_t s0x41_read_ir = {
    .slave_addr = 0x41,
    .command = 0x04,  // MB_FUNC_READ_INPUT_REGISTER
    .reg_start = 0x0000,
    .reg_size = IR_SIZE,
};

static mb_param_request_t s0x41_write_c = {
    .slave_addr = 0x41,
    .command = 0x05,  // MB_FUNC_WRITE_SINGLE_COIL
    .reg_start = 0x0000,
    .reg_size = 1,
};

static mb_param_request_t s0x41_write_hr = {
    .slave_addr = 0x41,
    .command = 0x06,  // MB_FUNC_WRITE_REGISTER
    .reg_start = 0x0000,
    .reg_size = 1,
};

void loop() {
    uint32_t ts_start_ms = esp_timer_get_time() / 1000;
    if (mb_state == MB_STATE::INIT) {
        print_arma_gripper();
        LCD.setCursor(4, TY_AG_NC);
        ui_set_text_params(RED);
        LCD.printf("INITIALIZATION FAILED!");
        // die
        while (true) {
            printf("ERROR: Initialization failed!\n");
            vTaskDelay(pdMS_TO_TICKS(10000));
        }
    }
    // read Input registers
    esp_err_t ret = mbc_master_send_request(&s0x41_read_ir, mb_res);
    bool offline = (ret != ESP_OK);
    if (ret == ESP_OK) {
        // update registers from RX buffer and mark changes
        update_registers(IR, IR_CHANGE, IR_SIZE);
    }

    // read Holding registers
    ret = mbc_master_send_request(&s0x41_read_hr, mb_res);
    offline &= (ret != ESP_OK);
    if (ret == ESP_OK) {
        // update registers from RX buffer and mark changes
        update_registers(HR, HR_CHANGE, HR_SIZE);
    }

    // read Coils
    ret = mbc_master_send_request(&s0x41_read_c, mb_res);
    offline &= (ret != ESP_OK);
    if (ret == ESP_OK) {
        // update coils from RX buffer and mark changes
        uint16_t r = mb_res[0];
        // printf("COILS: ");
        // for (int i = 0; i < 16; i++) {
        //     printf("%04X ", mb_res[i]);
        //     r |= (mb_res[i] & 0x01) << i;
        // }
        // printf("\n");
        COILS_CHANGE |= (r ^ COILS);
        COILS = r;
    }

    if (offline) {
        IR_CHANGE[IRA_ERROR_STATUS] |= (IR[IRA_ERROR_STATUS] != INVALID);
        IR[IRA_ERROR_STATUS] = INVALID;
    }

    M5.update();
    update_ui();
    uint32_t t_ms = esp_timer_get_time() / 1000 - ts_start_ms;
    if (t_ms < 33) vTaskDelay(pdMS_TO_TICKS(33 - t_ms));
}

extern "C" void app_main() {
    setup();
    for (;;) {
        // infinite loop
        loop();
        taskYIELD();
    }
}

static void ag_pos0() {
    s0x41_write_hr.reg_start = HRA_TARGET_POS;
    mb_res[0] = HR[HRA_POS0];
    mbc_master_send_request(&s0x41_write_hr, mb_res);
}

static void ag_pos1() {
    s0x41_write_hr.reg_start = HRA_TARGET_POS;
    mb_res[0] = HR[HRA_POS1];
    mbc_master_send_request(&s0x41_write_hr, mb_res);
}

static void ag_cm() {
    s0x41_write_c.reg_start = CA_CONTROL_MODE;
    mb_res[0] = (COILS & (1 << CA_CONTROL_MODE)) ? 0 : 0xFF00;
    mbc_master_send_request(&s0x41_write_c, mb_res);
}

static void ag_stop() {
    s0x41_write_c.reg_start = CA_STOP;
    mb_res[0] = (COILS & (1 << CA_STOP)) ? 0 : 0xFF00;
    mbc_master_send_request(&s0x41_write_c, mb_res);
}

static void ag_reboot() {
    s0x41_write_c.reg_start = CA_REBOOT;
    mb_res[0] = 0xFF00;
    mbc_master_send_request(&s0x41_write_c, mb_res);
}

static void ag_cal() {
    s0x41_write_c.reg_start = CA_START_CALIBRATION;
    mb_res[0] = 0xFF00;
    mbc_master_send_request(&s0x41_write_c, mb_res);
}

static void update_registers(uint16_t* regs, bool* changes, size_t size) {
    assert(regs);
    assert(changes);
    for (size_t i = 0; i < size; i++) {
        uint16_t r = mb_res[i];
        changes[i] |= (r != regs[i]);
        regs[i] = r;
    }
}

static void update_ui_online(bool force_draw) {
    if (force_draw || IR_CHANGE[IRA_DI] || IR_CHANGE[IRA_ERROR_STATUS]) {
        // status or error status Changed
        update_ui_error_status();
        update_ui_status();
    }

    if (force_draw || IR_CHANGE[IRA_CURRENT_POSITION] || HR_CHANGE[HRA_TARGET_POS]) {
        // Current or Target Position Changed
        update_ui_position();
    }

    if (force_draw || COILS_CHANGE) {
        // coils changed
        update_ui_coils();
    }

    for (size_t i = 0; i < BUTTONS_SIZE; i++) {
        // update button
        BUTTONS[i].btn->update(force_draw);
        if (BUTTONS[i].btn->isChangeToPressed()) {
            if (BUTTONS[i].action) BUTTONS[i].action();
        }
    }
}

static void update_ui(bool force_draw) {
    MB_STATE new_mb_state =
        (IR[IRA_ERROR_STATUS] == INVALID) ? MB_STATE::OFFLINE : MB_STATE::ONLINE;
    force_draw |= (new_mb_state != mb_state);
    mb_state = new_mb_state;
    if (force_draw) { LCD.fillScreen(BLACK); }
    if (mb_state == MB_STATE::ONLINE) {
        update_ui_online(force_draw);
    } else if (force_draw) {
        print_arma_gripper();
        LCD.setCursor(4, TY_AG_NC);
        ui_set_text_params(RED);
        LCD.printf("GRIPPER MCU NOT CONNECTED!");
    }
}

static void update_ui_error_status() {
    IR_CHANGE[IRA_ERROR_STATUS] = false;
    uint16_t es = IR[IRA_ERROR_STATUS];

    LCD.setCursor(0, TY_TITLE);
    if (es == 0) {
        // online with no error
        ui_set_text_params(WHITE);
        LCD.printf("        ARMA GRIPPER");
        ui_clear_rest_of_line();
        return;
    }
    ui_set_text_params(RED);
    if (es & 0xFFCE) {
        LCD.printf("HW ERROR!");
    } else if (es & 0x0001) {
        LCD.printf("POWER FAILURE!");
    } else if (es & 0x0010) {
        if (IR[IRA_DI] & (1 << DIA_CALIBRATION_IN_PROGRESS))
            LCD.printf("CALIBRATION IN PROGRESS!");
        else
            LCD.printf("NEED CALIBRATION!");
    } else if (es & 0x0020) {
        LCD.printf("MOTOR DRIVER OVER-TEMP!");
    }
    ui_clear_rest_of_line();
}

static void print_reg_value100(const char* label,
                               uint16_t reg,
                               uint16_t val_color = YELLOW,
                               uint16_t label_color = LIGHTGREY) {
    LCD.setTextColor(label_color, BLACK);
    if (label) LCD.printf("%s: ", label);
    if (reg == INVALID) {
        LCD.setTextColor(RED, BLACK);
        LCD.printf("INV");
    } else {
        LCD.setTextColor(val_color, BLACK);
        LCD.printf("%3lu", RV100(reg));
    }
}

static void update_ui_position() {
    IR_CHANGE[IRA_CURRENT_POSITION] = false;
    HR_CHANGE[HRA_TARGET_POS] = false;
    ui_set_text_params();
    LCD.setCursor(0, TY_POS);
    print_reg_value100("POSITION", IR[IRA_CURRENT_POSITION], WHITE);
    print_reg_value100("   T.POS", HR[HRA_TARGET_POS], YELLOW);
    ui_clear_rest_of_line();
}

static void update_ui_coils() {
    COILS_CHANGE = 0;
    ui_set_text_params(DARKGREY);
    LCD.setCursor(0, TY_COILS);
    LCD.printf("   ");
    LCD.setTextColor((COILS & (1 << CA_STOP)) ? ORANGE : DARKGREY, BLACK);
    LCD.printf(" STP ");
    if (COILS & (1 << CA_CONTROL_MODE)) {
        LCD.setTextColor(GREENYELLOW, BLACK);
        LCD.printf(" CMB ");
    } else {
        LCD.setTextColor(LIGHTGREY, BLACK);
        LCD.printf(" CIO ");
    }

    LCD.setTextColor((COILS & (1 << CA_REBOOT)) ? ORANGE : DARKGREY, BLACK);
    LCD.printf(" RBT ");

    if (COILS & (1 << CA_OUTPUT_ACTIVE_LEVEL)) {
        LCD.setTextColor(RED, BLACK);
        LCD.printf(" OAH ");
    } else {
        LCD.setTextColor(BLUE, BLACK);
        LCD.printf(" OAL ");
    }
}

static const uint16_t OUT_COLORS[] = { DARKGREY, RED, BLUE, MAGENTA };

static void update_ui_status() {
    IR_CHANGE[IRA_DI] = false;
    uint16_t st = IR[IRA_DI];

    ui_set_text_params(DARKGREY);
    LCD.setCursor(0, TY_STATUS);

    LCD.setTextColor((st & (1 << DIA_COLLISION)) ? ORANGE : DARKGREY, BLACK);
    LCD.printf("COL ");

    LCD.setTextColor((st & (1 << DIA_IN0)) ? GREENYELLOW : DARKGREY, BLACK);
    LCD.printf("IN0 ");
    LCD.setTextColor((st & (1 << DIA_IN1)) ? GREENYELLOW : DARKGREY, BLACK);
    LCD.printf(" IN1 ");

    LCD.setTextColor(OUT_COLORS[(st & (3 << DIA_OUT0H)) >> DIA_OUT0H], BLACK);
    LCD.printf(" OP0 ");
    LCD.setTextColor(OUT_COLORS[(st & (3 << DIA_OUT1H)) >> DIA_OUT1H], BLACK);
    LCD.printf(" OP1 ");

    LCD.setTextColor((st & (1 << DIA_WARN_MDT)) ? ORANGE : DARKGREY, BLACK);
    LCD.printf("MDT");
}

static void print_arma_gripper() {
    ui_set_text_params(WHITE, BLACK, 1, 4);
    LCD.setCursor(20, 100);
    LCD.printf("ARMA Gripper");
}
