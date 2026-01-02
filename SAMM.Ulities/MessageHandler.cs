namespace SAMM.Utilities
{
	public static class MessageHandler
	{
		public delegate void MessagePopupEventHandler(string header, string message);
		private static event MessagePopupEventHandler? MessagePopupCallback;

		public static void Initialize(MessagePopupEventHandler popupEventFunc)
		{
			MessagePopupCallback += popupEventFunc;
		}

		public static void OpenDialog(string header, string message)
		{
			if (MessagePopupCallback != null)
				MessagePopupCallback(header, message);
		}
	}
}
