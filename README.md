# LatencySpy

Is a VST3 x64 pluginproject about a plugin that is reporting if other plugins between 2 Latency Spys are adding more latency PDC Plugin Delay Correction, so that the host/DAW must compensate. The Latency Spy will help you find out which plugins are doing just that additional delay/latency.
The plugin is also measuring the time from the first Latency Spy in line starts to do its work until the next one, this is presented as the RPL Realtime Plugin Latency in each Latency Spy. The further down the chain the more RPL there will be.

This plugin will not show any PDC in Sonar 8 because Sonar doesnt report PDC right apparently.

Here is a tutorial over how to use the plugin: https://www.youtube.com/watch?v=ZeVDHgCQW8c

Under the GPL-3.0 license

VST® is a trademark of Steinberg Media Technologies GmbH, registered in Europe and other countries.
