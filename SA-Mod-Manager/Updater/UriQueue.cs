using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Pipes;
using System.Linq;

namespace SAModManager.Updater
{
    public class OnUriEnqueuedArgs : EventArgs
    {
        public string Uri { get; }
        public bool Handled;

        public OnUriEnqueuedArgs(string uri)
        {
            Uri = uri;
        }
    }

    public class UriQueue
    {
        private bool running = true;
        private readonly Queue<string> lines = new Queue<string>();
        private readonly string pipeName;

        public event EventHandler<OnUriEnqueuedArgs> UriEnqueued;

        public UriQueue(string pipeName)
        {
            this.pipeName = pipeName;
            Start();
        }

        private void Start()
        {
            if (!running)
            {
                return;
            }

            NamedPipeServerStream pipe;

            try
            {
                pipe = new NamedPipeServerStream(pipeName, PipeDirection.In, 10, PipeTransmissionMode.Byte, PipeOptions.Asynchronous);
            }
            catch (IOException)
            {
                // IOException is thrown when the maximum allowed instances is reached.
                // Just ignore anything past the specified amount and continue.
                return;
            }

            pipe.BeginWaitForConnection(HandleConnection, pipe);
        }

        private void HandleConnection(IAsyncResult ar)
        {
            using (var conn = (NamedPipeServerStream)ar.AsyncState)
            {
                try
                {
                    conn.EndWaitForConnection(ar);
                }
                catch (Exception)
                {
                    // ignored
                    return;
                }

                // this is not quite recursive
                Start();

                var reader = new StreamReader(conn);

                while (conn.IsConnected && running)
                {
                    string line;

                    try
                    {
                        line = reader.ReadLine();
                    }
                    catch (IOException)
                    {
                        // just abort now
                        break;
                    }

                    if (string.IsNullOrEmpty(line))
                    {
                        break;
                    }

                    if (OnUriEnqueued(new OnUriEnqueuedArgs(line)))
                    {
                        continue;
                    }

                    lock (lines)
                    {
                        lines.Enqueue(line);
                    }
                }
            }
        }

        public void Close()
        {
            running = false;
        }

        public List<string> GetUris()
        {
            List<string> result;
            lock (lines)
            {
                result = lines.ToList();
                lines.Clear();
            }
            return result;
        }

        private bool OnUriEnqueued(OnUriEnqueuedArgs e)
        {
            UriEnqueued?.Invoke(this, e);
            return e.Handled;
        }
    }
}
