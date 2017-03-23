using System;
using System.Windows.Forms;

namespace SADXModManager.Forms
{
	public partial class ProgressDialog : Form
	{
		public event EventHandler CancelEvent;
		#region Accessors

		/// <summary>
		/// Gets or sets the current task displayed on the window. (Upper label)
		/// </summary>
		public string CurrentTask => labelTask.Text;

		/// <summary>
		/// Gets or sets the current step displayed on the window. (Lower label)
		/// </summary>
		public string CurrentStep => labelStep.Text;

		/// <summary>
		/// Gets or sets the title of the window.
		/// </summary>
		public string Title
		{
			get => Text;
			set => Text = value;
		}

		// HACK: This is a work around for slow progress bar animation.
		private int progressValue
		{
			get => progressBar.Value;
			set
			{
				if (value > progressBar.Maximum)
				{
					progressBar.Value = progressBar.Maximum;
					return;
				}

				++progressBar.Maximum;
				progressBar.Value = value + 1;
				progressBar.Value = value;
				--progressBar.Maximum;
			}
		}

		#endregion

		private int taskCount;
		private int taskIndex;
		private double multiplier;

		/// <summary>
		/// Initializes a ProgressDialog which displays the current task, the step in that task, and a progress bar.
		/// </summary>
		/// <param name="title">The title of the window</param>
		/// <param name="taskCount">Number of tasks this dialog will handle.</param>
		/// <param name="allowCancel">Enables or disables the cancel button.</param>
		public ProgressDialog(string title, int taskCount, bool allowCancel)
			: this(title, allowCancel)
		{
			SetTaskCount(taskCount);
		}

		/// <summary>
		/// Initializes a ProgressDialog which displays the current task, the step in that task, and a progress bar.
		/// </summary>
		/// <param name="title">The title of the window</param>
		/// <param name="allowCancel">Enables or disables the cancel button.</param>
		public ProgressDialog(string title, bool allowCancel)
		{
			InitializeComponent();

			Title = title;
			labelTask.Text = "";
			labelStep.Text = "";
			buttonCancel.Enabled = allowCancel;
		}

		public void SetTaskCount(int count)
		{
			if (InvokeRequired)
			{
				Invoke((Action<int>)SetTaskCount, count);
				return;
			}

			taskCount = count;
			multiplier = progressBar.Maximum / (double)count;
		}

		public void NextTask()
		{
			if (InvokeRequired)
			{
				Invoke((Action)NextTask);
				return;
			}

			if (taskIndex + 1 < taskCount)
			{
				++taskIndex;
				progressValue = (int)(taskIndex * multiplier);
				return;
			}

			progressBar.Value = progressBar.Maximum;
			Close();
		}

		public void SetProgress(double value)
		{
			if (InvokeRequired)
			{
				Invoke((Action<double>)SetProgress, value);
				return;
			}

			progressValue = (int)(taskIndex * multiplier + value / multiplier);
		}

		/// <summary>
		/// Sets the current task to display on the window. (Upper label)
		/// </summary>
		/// <param name="text">The string to display as the task.</param>
		public void SetTask(string text)
		{
			if (InvokeRequired)
			{
				Invoke((Action<string>)SetTask, text);
			}
			else
			{
				labelTask.Text = text;
			}
		}

		/// <summary>
		/// Sets the current step to display on the window. (Lower label)
		/// </summary>
		/// <param name="text">The string to display as the step.</param>
		public void SetStep(string text)
		{
			if (InvokeRequired)
			{
				Invoke((Action<string>)SetStep, text);
			}
			else
			{
				labelStep.Text = text;
			}
		}

		/// <summary>
		/// Sets the task and step simultaneously.
		/// Both parameters default to null, so you may also use this to clear them.
		/// </summary>
		/// <param name="task">The string to display as the task. (Upper label)</param>
		/// <param name="step">The string to display as the step. (Lower label)</param>
		public void SetTaskAndStep(string task = null, string step = null)
		{
			if (InvokeRequired)
			{
				Invoke((Action<string, string>)SetTaskAndStep, task, step);
			}
			else
			{
				labelTask.Text = task;
				labelStep.Text = step;
			}
		}

		private void ProgressDialog_Load(object sender, EventArgs e)
		{
			CenterToParent();
			buttonCancel.Select();
		}

		private void buttonCancel_Click(object sender, EventArgs e)
		{
			if (MessageBox.Show(this, "Are you sure you want to cancel?", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Question) != DialogResult.Yes)
			{
				DialogResult = DialogResult.None;
				return;
			}

			OnCancelEvent();
			DialogResult = DialogResult.Cancel;
			Close();
		}

		protected virtual void OnCancelEvent()
		{
			CancelEvent?.Invoke(this, EventArgs.Empty);
		}
	}
}
