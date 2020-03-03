
#include "bluetooth.h"
#include "main.h"
#include "definitions.h"
#include "draw.h"
#include "ws2812.h"
#include "minihdlc-master/minihdlc.h"

void sendDataBluetooth(uint8_t data)
{
  while (!LL_USART_IsActiveFlag_TXE(USART1))
  {
  }
  LL_USART_TransmitData8(USART1, data);
}

void setBackFunction(uint8_t mode)
{
  switch (mode)
  {
  case MODE_BACK_CIRCLE:
    drawshape_function = drawShapeNote;
    callback_function = circle;
    break;
  case MODE_BACK_CIRCLE_GRAD:
    drawshape_function = drawShapeNoteGrad;
    callback_function = circle;
    break;
  case MODE_BACK_CROSS:
    drawshape_function = drawShapeNote;
    callback_function = cross;
    break;
  case MODE_BACK_CROSS_GRAD:
    drawshape_function = drawShapeNoteGrad;
    callback_function = cross;
    break;
  case MODE_BACK_SQUARE:
    drawshape_function = drawShapeNote;
    callback_function = square;
    break;
  case MODE_BACK_SQUARE_GRAD:
    drawshape_function = drawShapeNoteGrad;
    callback_function = square;
    break;
  case MODE_BACK_TRIANGLE:
    drawshape_function = drawShapeNote;
    callback_function = triangle;
    break;
  case MODE_BACK_TRIANGLE_GRAD:
    drawshape_function = drawShapeNoteGrad;
    callback_function = triangle;
    break;
  case MODE_BACK_LINES_H:
    drawshape_function = drawShapeNote;
    callback_function = horizontal;
    break;
  case MODE_BACK_LINES_H_GRAD:
    drawshape_function = drawShapeNoteGrad;
    callback_function = horizontal;
    break;
  case MODE_BACK_LINES_V:
    drawshape_function = drawShapeNote;
    callback_function = vertical;
    break;
  case MODE_BACK_LINES_V_GRAD:
    drawshape_function = drawShapeNoteGrad;
    callback_function = vertical;
    break;
  }
}

void setColorVariable(uint8_t color)
{
  switch (color)
  {
  case MODE_BACK_COLORS_OG:
    arrColor = &colorOriginal[0];
    break;
  case MODE_BACK_COLORS_BW:
    arrColor = &colorBW[0];
    break;
  }
}

void receiveDataBluetooth(const uint8_t *frame_buffer, uint16_t frame_length)
{
  uint8_t data;
  data = *frame_buffer++;
  drawtext_function = drawNote;
  switch (data)
  {
  case SETTINGS_BRIGHT:
    data = *frame_buffer++;
    MAX_BRIGHT = (float)data / 20;
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);
    break;
  case MODE_BACK_STATE:
    data = *frame_buffer++;
    MODE_BACK = data;
    clearBack();
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);
    break;
  case MODE_TEXT_STATE:
    data = *frame_buffer++;
    MODE_TEXT = data;
    clearText();
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);
    break;
  case MODE_CUSTOM:
    data = *frame_buffer++;
    MAX_BRIGHT = (float)data / 20;
    data = *frame_buffer++;
    MODE_BACK = data;
    data = *frame_buffer++;
    setBackFunction(data);
    data = *frame_buffer++;
    MAX_DIV = data;
    data = *frame_buffer++;
    orientation = data;
    data = *frame_buffer++;
    MODE_TEXT = data;
    data = *frame_buffer++;
    setColorVariable(data);
    fillFramebuffer();
    WS2812_sendbuf(24 * NLEDSCH);
    break;
  default:
    if (data > 0x20 && data < 0x50)
    {
      setBackFunction(data);
      data = *frame_buffer++;
      MAX_DIV = data;
      data = *frame_buffer++;
      orientation = data;
      minihdlc_send_frame(ACK, 1);
    }
    else if (data > 0x50 && data < 0x60)
    {
      setColorVariable(data);
      minihdlc_send_frame(ACK, 1);
    }
    break;
  }
}

void startReception(void)
{
  minihdlc_init(sendDataBluetooth, receiveDataBluetooth);
  /* Clear Overrun flag, in case characters have already been sent to USART */
  LL_USART_ClearFlag_ORE(USART1);

  /* Enable RXNE and Error interrupts */
  LL_USART_EnableIT_RXNE(USART1);
  LL_USART_EnableIT_ERROR(USART1);
  uint8_t buffer[] = "OLA amigo!";

  minihdlc_send_frame(buffer, ARRAY_LEN(buffer));
}

void USART1_IRQHandler(void)
{
  uint8_t RxBuffer;
  if (LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
  {
    // LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    RxBuffer = LL_USART_ReceiveData8(USART1);
    minihdlc_char_receiver(RxBuffer);
    sendDataBluetooth(RxBuffer);
  }
}
