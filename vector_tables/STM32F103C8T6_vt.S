/*
 * Minimal vector table for an STM32F103C8 / STM32F103CB.
 */

.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

// Global values.
.global vtable
.global default_interrupt_handler

.type vtable, %object
.section .vector_table,"a",%progbits
vtable:
    // 0-15
    .word _estack
    .word reset_handler
    .word NMI_handler
    .word hard_fault_handler
    .word mem_manage_handler
    .word bus_fault_handler
    .word usage_fault_handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_handler
    .word debug_monitor_handler
    .word 0
    .word pending_SV_handler
    .word SysTick_handler
    // 16-31
    .word window_watchdog_IRQ_handler
    .word PVD_IRQ_handler
    .word tamper_IRQ_handler
    .word RTC_IRQ_handler
    .word flash_IRQ_handler
    .word RCC_IRQ_handler
    .word EXTI0_IRQ_handler
    .word EXTI1_IRQ_handler
    .word EXTI2_IRQ_handler
    .word EXTI3_IRQ_handler
    .word EXTI4_IRQ_handler
    .word DMA1_chan1_IRQ_handler
    .word DMA1_chan2_IRQ_handler
    .word DMA1_chan3_IRQ_handler
    .word DMA1_chan4_IRQ_handler
    // 32-47
    .word DMA1_chan5_IRQ_handler
    .word DMA1_chan6_IRQ_handler
    .word DMA1_chan7_IRQ_handler
    .word ADC1_2_IRQ_handler
    .word USB_HP_CAN1_TX_IRQ_handler
    .word USB_LP_CAN1_RX0_IRQ_handler
    .word CAN1_RX1_IRQ_handler
    .word CAN1_SCE_IRQ_handler
    .word EXTI5_9_IRQ_handler
    .word TIM1_break_IRQ_handler
    .word TIM1_UP_IRQ_handler
    .word TIM1_trigger_IRQ_handler
    .word TIM1_CC_IRQ_handler
    .word TIM2_IRQ_handler
    .word TIM3_IRQ_handler
    .word TIM4_IRQ_handler
    // 48-63
    .word I2C1_EV_IRQ_handler
    .word I2C1_ER_IRQ_handler
    .word I2C2_EV_IRQ_handler
    .word I2C2_ER_IRQ_handler
    .word SPI1_IRQ_handler
    .word SPI2_IRQ_handler
    .word USART1_IRQ_handler
    .word USART2_IRQ_handler
    .word USART3_IRQ_handler
    .word EXTI10_15_IRQ_handler
    .word RTC_alarm_IRQ_handler
    .word USB_wakeup_IRQ_handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0

    /*
     * Setup weak aliases for each exception handler to the
     * default one. These can be updated later, or just
     * overridden since they're weak refs.
     * The reset_handler is set up separately.
     */
    .weak NMI_handler
    .thumb_set NMI_handler, pass_interrupt_handler
    .weak hard_fault_handler
    .thumb_set hard_fault_handler, default_interrupt_handler
    .weak mem_manage_handler
    .thumb_set mem_manage_handler, default_interrupt_handler
    .weak bus_fault_handler
    .thumb_set bus_fault_handler, default_interrupt_handler
    .weak usage_fault_handler
    .thumb_set usage_fault_handler, default_interrupt_handler
    .weak SVC_handler
    .thumb_set SVC_handler, pass_interrupt_handler
    .weak debug_monitor_handler
    .thumb_set debug_monitor_handler, default_interrupt_handler
    .weak pending_SV_handler
    .thumb_set pending_SV_handler, pass_interrupt_handler
    .weak SysTick_handler
    .thumb_set SysTick_handler, pass_interrupt_handler
    .weak window_watchdog_IRQ_handler
    .thumb_set window_watchdog_IRQ_handler, pass_interrupt_handler
    .weak PVD_IRQ_handler
    .thumb_set PVD_IRQ_handler, default_interrupt_handler
    .weak tamper_IRQ_handler
    .thumb_set tamper_IRQ_handler, default_interrupt_handler
    .weak RTC_IRQ_handler
    .thumb_set RTC_IRQ_handler, default_interrupt_handler
    .weak flash_IRQ_handler
    .thumb_set flash_IRQ_handler, default_interrupt_handler
    .weak RCC_IRQ_handler
    .thumb_set RCC_IRQ_handler, default_interrupt_handler
    .weak EXTI0_IRQ_handler
    .thumb_set EXTI0_IRQ_handler, default_interrupt_handler
    .weak EXTI1_IRQ_handler
    .thumb_set EXTI1_IRQ_handler, default_interrupt_handler
    .weak EXTI2_IRQ_handler
    .thumb_set EXTI2_IRQ_handler, default_interrupt_handler
    .weak EXTI3_IRQ_handler
    .thumb_set EXTI3_IRQ_handler, default_interrupt_handler
    .weak EXTI4_IRQ_handler
    .thumb_set EXTI4_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan1_IRQ_handler
    .thumb_set DMA1_chan1_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan2_IRQ_handler
    .thumb_set DMA1_chan2_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan3_IRQ_handler
    .thumb_set DMA1_chan3_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan4_IRQ_handler
    .thumb_set DMA1_chan4_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan5_IRQ_handler
    .thumb_set DMA1_chan5_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan6_IRQ_handler
    .thumb_set DMA1_chan6_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan7_IRQ_handler
    .thumb_set DMA1_chan7_IRQ_handler, default_interrupt_handler
    .weak ADC1_2_IRQ_handler
    .thumb_set ADC1_2_IRQ_handler, default_interrupt_handler
    .weak USB_HP_CAN1_TX_IRQ_handler
    .thumb_set USB_HP_CAN1_TX_IRQ_handler, default_interrupt_handler
    .weak USB_LP_CAN1_RX0_IRQ_handler
    .thumb_set USB_LP_CAN1_RX0_IRQ_handler, default_interrupt_handler
    .weak CAN1_RX1_IRQ_handler
    .thumb_set CAN1_RX1_IRQ_handler, default_interrupt_handler
    .weak CAN1_SCE_IRQ_handler
    .thumb_set CAN1_SCE_IRQ_handler, default_interrupt_handler
    .weak EXTI5_9_IRQ_handler
    .thumb_set EXTI5_9_IRQ_handler, default_interrupt_handler
    .weak TIM1_break_IRQ_handler
    .thumb_set TIM1_break_IRQ_handler, default_interrupt_handler
    .weak TIM1_UP_IRQ_handler
    .thumb_set TIM1_UP_IRQ_handler, default_interrupt_handler
    .weak TIM1_trigger_IRQ_handler
    .thumb_set TIM1_trigger_IRQ_handler, default_interrupt_handler
    .weak TIM1_CC_IRQ_handler
    .thumb_set TIM1_CC_IRQ_handler, default_interrupt_handler
    .weak TIM2_IRQ_handler
    .thumb_set TIM2_IRQ_handler, default_interrupt_handler
    .weak TIM3_IRQ_handler
    .thumb_set TIM3_IRQ_handler, default_interrupt_handler
    .weak TIM4_IRQ_handler
    .thumb_set TIM4_IRQ_handler, default_interrupt_handler
    .weak I2C1_EV_IRQ_handler
    .thumb_set I2C1_EV_IRQ_handler, default_interrupt_handler
    .weak I2C1_ER_IRQ_handler
    .thumb_set I2C1_ER_IRQ_handler, default_interrupt_handler
    .weak I2C2_EV_IRQ_handler
    .thumb_set I2C2_EV_IRQ_handler, default_interrupt_handler
    .weak I2C2_ER_IRQ_handler
    .thumb_set I2C2_ER_IRQ_handler, default_interrupt_handler
    .weak SPI1_IRQ_handler
    .thumb_set SPI1_IRQ_handler, default_interrupt_handler
    .weak SPI2_IRQ_handler
    .thumb_set SPI2_IRQ_handler, default_interrupt_handler
    .weak USART1_IRQ_handler
    .thumb_set USART1_IRQ_handler, default_interrupt_handler
    .weak USART2_IRQ_handler
    .thumb_set USART2_IRQ_handler, default_interrupt_handler
    .weak USART3_IRQ_handler
    .thumb_set USART3_IRQ_handler, default_interrupt_handler
    .weak EXTI10_15_IRQ_handler
    .thumb_set EXTI10_15_IRQ_handler, default_interrupt_handler
    .weak RTC_alarm_IRQ_handler
    .thumb_set RTC_alarm_IRQ_handler, default_interrupt_handler
    .weak USB_wakeup_IRQ_handler
    .thumb_set USB_wakeup_IRQ_handler, default_interrupt_handler
.size vtable, .-vtable

/*
 * A 'Default' interrupt handler. This is where interrupts
 * which are not otherwise configured will go.
 * It is an infinite loop, because...well, clearly we weren't
 * expecting the interrupt, so what can we do?
 */
.section .text.default_interrupt_handler,"ax",%progbits
default_interrupt_handler:
    default_interrupt_loop:
      B default_interrupt_loop
.size default_interrupt_handler, .-default_interrupt_handler

.section .text.pass_interrupt_handler,"ax",%progbits
pass_interrupt_handler:
  BX lr
.size pass_interrupt_handler, .-pass_interrupt_handler
