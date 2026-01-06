namespace SAMM.Utilities
{
	public static class MessageHandler
	{
		public delegate bool MessagePopupEventHandler(string header, string message);
		private static event MessagePopupEventHandler? MessagePopupCallback;

		public static void Initialize(MessagePopupEventHandler popupEventFunc)
		{
			MessagePopupCallback += popupEventFunc;
		}

		public static bool OpenDialog(string header, string message)
		{
			if (MessagePopupCallback != null)
				return MessagePopupCallback(header, message);

			return false;
		}
	}
}
