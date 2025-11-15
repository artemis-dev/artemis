# Crafting Steering Files

Steering files tell ARTEMIS how to assemble and run an analysis loop. They are plain YAML documents interpreted by `TLoopManager`, so keeping a predictable structure and naming convention makes it easy to swap processors, share templates, and automate runs via the CLI.

## Global Structure

A steering file is a YAML map with a top-level `Processor` key whose value is an ordered sequence. Each item describes one processor instance: its unique `name`, the C++ `type` to instantiate, and optional configuration payload. This keeps the runtime loop deterministic—processors are created and executed in list order.

```yaml
Processor:
  - name: random
    type: art::TRandomNumberEventStore
    parameter:
      MaxLoop: 10000000000
```

The example above configures a generator, followed by additional processors (timer, output tree, histogram projections) that read the collections created upstream. Using a different event store, such as `art::TRIDFEventStore`, swaps in real RIDF files without altering downstream modules.

### Standard Keys Per Processor

| Key | Required? | Purpose |
| --- | --- | --- |
| `name` | Yes | Unique instance name. Displayed in logs and used when writing metadata to `TAnalysisInfo`. |
| `type` | Yes | Fully qualified processor class registered with ROOT. Must inherit from `art::TProcessor`. |
| `title` | No | Optional human-readable description; maps to the `TNamed` title. |
| `parameter` | No | Supplies configuration values. See below for supported formats. |
| `input`, `output`, `info` | No | Some processors expose helper arrays/collections via named parameters; declaring them in YAML connects collections registered with `RegisterInputCollection`, `RegisterOutputCollection`, or `RegisterProcessorParameter`.

## Declaring Parameters

Processor constructors register parameters (scalar values, strings, vectors, or object references) via `RegisterProcessorParameter`. The YAML `parameter` node can be authored in two ways:

1. **Map style** — easiest for scalar options. Each child key maps directly onto a registered parameter, as seen in `rndmEventStore.yaml` for `MaxLoop`, `OutputCollection`, `SplitLevel`, or tree projection settings.
2. **Sequence style** — useful when order matters or when you need to repeat the same key. Each entry is a map containing `name` and `value`. Values may themselves be scalars or sequences; for example `ridfEventStore.yaml` lists multiple input files by giving `value` a nested sequence.

Regardless of syntax, YAML values are assigned verbatim to the processor’s registered parameter types at initialization (`TProcessor::InitProc`). ARTEMIS enforces that required parameters are present and reports descriptive errors when they are missing or of the wrong type.

### Referring to External Definitions

Some processors, such as `art::TTreeProjectionProcessor`, expect file paths (e.g., `FileName: histdef.yaml`) so they can load histogram or projection definitions. Those auxiliary YAML files live outside the steering document and can themselves use macro replacement.

## Template Inclusion and Macro Replacement

Large analyses often reuse YAML snippets. ARTEMIS supports two complementary mechanisms:

1. **CLI macro replacement** – The `add` command accepts `key=value` arguments (or a bare integer). At load time, TLoop substitutes every occurrence of `@key@` with the provided value and stores the substitutions inside `TAnalysisInfo`, letting you reuse a single steering template across run numbers. The special placeholder `@NUM@` is replaced when you pass a lone integer argument, which is handy for per-run configuration.

2. **In-file `include` blocks** – Inside the `Processor` sequence you may insert an `include` node instead of a processor definition. When the node is a scalar string, TLoop simply loads the referenced YAML (relative to the current file) and splices in its `Processor` list. When it is a map, you can provide `name` (the file to load) plus an optional `replace` map. Every `@key@` token inside the included file is substituted by the corresponding scalar or sequence before parsing, enabling powerful templating for repeated processor bundles. Sequence replacements are written with JSON-like `["value1","value2"]` syntax generated automatically by ARTEMIS.

To prevent recursive inclusion loops, TLoop keeps track of inode numbers for every file it reads and aborts if a file would be included twice by reference. The event metadata (`analysisInfo`) also records the final, expanded YAML for reproducibility.

## Event Stores and Downstream Processors

The first processor in a steering file is typically an event store (e.g., `art::TRandomNumberEventStore`, `art::TRIDFEventStore`, `art::TTreeEventStore`). These classes publish collections into the shared `TEventCollection`. Subsequent processors should refer to those collection names via their parameters. For example, the histogram-projection processor consumes whatever branches `TOutputTreeProcessor` wrote, and the timer module simply observes the loop progress. Choose the event store that matches your data source—random numbers for testing, RIDF streams for actual data, or custom implementations compiled into `libuser`.

## Recording Metadata

During `Init`, TLoop stores a serialized copy of the resolved processor list under `/artemis/loops/loopN/Config` and annotates `TAnalysisInfo` with the steering file path, macro replacements, start/end timestamps, run information (if provided by an event store), and arbitrary string data. This makes every run reproducible and easy to audit later.

## Practical Tips

- Keep processor names descriptive (e.g., `ridfevent`, `outputtree`) so log messages and ROOT folders are self-explanatory.
- Group related processors into include files (`daq.yaml`, `calibration.yaml`, etc.) and pass `replace` values to customize target collections per run.
- Store histogram definitions (`histdef.yaml`) separately so you can iterate on binning without touching the main steering document.
- When testing, use the random event store template; swap to RIDF inputs by changing only the first processor and its parameters.

Following these conventions will keep steering files consistent, modular, and easy to reason about even as analyses grow to dozens of processors.
