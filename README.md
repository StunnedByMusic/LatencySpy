# LatencySpy

Is a VST3 x64 pluginproject that is reporting if other plugins in between 2 Latency Spys are adding more latency behind the scenes as for an example in a predelay plugin or any plugin that might do a lookahead, so that the host must do the PDC (Plugin Delay Correction).
In that case the host/DAW must compensate for the time needed so that the plugin will be able to look into the future, and the downside of that is more latency/delay.
The Latency Spy will help you find out which plugins are doing just that additional delay/latency.

The plugin is also measuring the time from when the first Latency Spy in line starts to do its work until the next one, this is presented as the RPL Realtime Plugin Latency in each Latency Spy. The further down the chain the more RPL there will be, if this value is high then this means that the workload in between is high.

This plugin will not show any PDC in Sonar 8 because Sonar doesnt report PDC right apparently.

Here is a tutorial over how to use the plugin: https://www.youtube.com/watch?v=ZeVDHgCQW8c

Under the GPL-3.0 license

VST® is a trademark of Steinberg Media Technologies GmbH, registered in Europe and other countries.
