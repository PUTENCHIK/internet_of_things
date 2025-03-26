# notifier.py
import os
import telebot

# Load environment variables
TELEGRAM_BOT_TOKEN = os.getenv("TELEGRAM_BOT_TOKEN")
TELEGRAM_CHAT_ID = os.getenv("TELEGRAM_CHAT_ID")
if TELEGRAM_CHAT_ID:
    chat_ids = [TELEGRAM_CHAT_ID]
else:
    chat_ids = []

# Validate environment variables
if not TELEGRAM_BOT_TOKEN:
    raise ValueError("TELEGRAM_BOT_TOKEN is not set in environment variables.")

bot = telebot.TeleBot(TELEGRAM_BOT_TOKEN)


def start_bot():
    """Runs the Telegram bot polling"""
    bot.polling(none_stop=True)


@bot.message_handler(commands=['start'])
def handle_start(message: telebot.types.Message):
    bot.reply_to(message, "This bot will notify you if some IoT device registered")


@bot.message_handler(commands=['chatid'])
def handle_chatid(message: telebot.types.Message) -> None:
    global chat_ids
    bot.reply_to(message, f"Your chat id is {message.chat.id}")
    chat_ids += [message.chat.id]
    send_telegram_notification("Hello there again")


def send_telegram_notification(message: str):
    """Send a notification message to the configured Telegram chat."""
    for chat_id in chat_ids:
        try:
            bot.send_message(chat_id, message)
        except Exception as e:
            print(f"Error sending Telegram notification: {e}")

# if __name__ == "__main__":
#     send_telegram_notification("Hello there?")
#     bot.infinity_polling()