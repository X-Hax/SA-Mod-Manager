using System;
using System.Buffers;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Threading;

namespace SAModManager
{
    public static class Extensions
    {

        public static async Task CopyToAsync(this HttpContent content, Stream destination, IProgress<double?> progress, CancellationToken cancellationToken = default)
        {
            progress?.Report(null);
            using (var source = await content.ReadAsStreamAsync().ConfigureAwait(false))
            {
                // adapted from CoreFX 
                // https://source.dot.net/#System.Private.CoreLib/Stream.cs,8048a9680abdd13b

                if (source is null)
                    throw new ArgumentNullException(nameof(source));
                if (destination is null)
                    throw new ArgumentNullException(nameof(destination));

                var contentLength = content.Headers.ContentLength;
                var bufferLength = 65536; // 64K seams reasonable?
                var buffer = ArrayPool<byte>.Shared.Rent(bufferLength); // prevents allocation of garbage arrays
                try
                {
                    int bytesRead;
                    int totalBytes = 0;
                    while ((bytesRead = await source.ReadAsync(buffer, 0, bufferLength, cancellationToken).ConfigureAwait(false)) != 0)
                    {
                        totalBytes += bytesRead;
                        await destination.WriteAsync(buffer, 0, bytesRead, cancellationToken).ConfigureAwait(false);

                        if (contentLength.HasValue)
                        {
                            double pourcent = (double)totalBytes / contentLength.Value;
                            progress?.Report(pourcent);
                        }
                        else
                        {
                            progress?.Report(null);
                        }
                    }
                }
                finally
                {
                    ArrayPool<byte>.Shared.Return(buffer);
                }
            }
        }


    }

}
