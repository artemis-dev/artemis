# ARTEMIS User Guide

## 1. Overview
ARTEMIS (A RooT Extension with Modular processor for Instant Switching) is a ROOT-based analysis framework that promotes modular processors, steering files, a parenthesis-free command-line interface, and multi-tree projection support so you can swap analysis snippets quickly and script complex flows interactively.

## 2. Installation & Requirements
ARTEMIS requires ROOT builds that include the Minuit2 and Geometry modules, plus a shared-library build of YAML-CPP for loading steering and histogram definition files (`cmake -DBUILD_SHARED_LIBS=ON`). Install by downloading the latest release or development snapshot, running `autoreconf --install`, configuring with `--with-yaml-cpp=<path>`, then compiling and installing with `make && make install`. Ensure the required ROOT components are available or enabled on your platform.

## 3. Launching the CLI
Invoke the `artemis` binary to start an interactive TRint session; when MPI support is enabled you can pass MPI-related options before the event loop begins, otherwise only the optional `-u` flag (username) is parsed before handing control to `TArtRint`. The interpreter drives `TLoopManager` so you can queue loops and issue commands interactively. A typical quick-start session looks like:

```
% artemis
artemis [0] add rndmEventStore.yaml
artemis [1] resume
artemis [2] suspend
artemis [3] terminate
```

Use `add` to load a steering file, `resume` to start processing, `suspend` to pause, and `terminate` to shut down the loop when finished.

## 4. Startup Customization
On startup ARTEMIS evaluates `artemislogon.C`. The default example script extends ROOT’s dynamic path to find your custom `processors` directory, loads `libuser`, registers all built-in commands with the `TCatCmdFactory` (help, histogram navigation, loop control, file commands, etc.), adjusts the include path for your processors, and registers module decoders so RIDF data can be understood. Adapt this file to preload your own libraries, commands, and hardware decoders before entering the CLI.

## 5. Event Loops & Steering Files
`TLoopManager` controls up to ten concurrent loops, exposing `Add`, `Resume`, `Suspend`, `Terminate`, and `GetLoop` operations to the command layer. Each `TLoop` owns a condition bitset (run/begin/end/stop flags), a `TEventCollection`, the ordered processors you define in YAML, and optional references to `TAnalysisInfo` and the active event store. Loops call `Load` / `LoadYAMLNode` to parse steering documents, track their ID, and expose metadata. `TAnalysisInfo` objects added to ROOT directories record the steering file name, run number/name, analysis start and end times, processors in use, and arbitrary string data so you can document each run succinctly and even merge the metadata later.

## 6. Crafting Steering Files
Steering files are YAML documents defining an ordered `Processor` list. Each entry names the processor instance, specifies its C++ type, and optionally sets parameters. For example, `example/rndmEventStore.yaml` configures two random-number event stores (with configurable loop limits and output collections), a timer, an output tree writer, and a tree projection processor that references `histdef.yaml`. Swap in real detector data by selecting another event store: `example/ridfEventStore.yaml` instantiates `art::TRIDFEventStore`, points it to one or more RIDF files, and attaches downstream processors to handle timing, random-number injections, or tree output as needed.

Event stores encapsulate data sources. `TRandomNumberEventStore` generates synthetic floating-point data between configurable bounds and publishes it under an output collection, while `TRIDFEventStore` opens offline or online RIDF streams, decodes segments via module decoders, keeps scaler and timestamp data, and exposes run header information through its input/output info objects. Choose the store that matches your data and bind it to downstream processors using collection names defined in the YAML.

## 7. Histogram Definitions & Tree Projections
Histogram definitions live in separate YAML files consumed by `art::TTreeProjectionProcessor`. `example/histdef.yaml` demonstrates the structure: top-level `group` entries collect named projections, each specifying a title and a projection formula (e.g., fill a histogram with `random.fValue` using 100 bins from 0 to 1). The projection processor accepts parameters for the definition file, projection name, and type; at initialization it loads the YAML, optionally runs macro replacements specified via the `Replace` parameter, builds a temporary `TTree` containing every object from the current `TEventCollection`, synchronizes projection formulas with the tree, and registers the projection object under `/artemis/loops/loop0`. During `Process` it copies one entry into the temporary tree and fills the configured projections, and at `PostLoop` it can persist histograms via `TCatCmdHstore`, optionally generating per-rank files and merging them when MPI is active. Use this mechanism to manage large sets of histogram definitions independently from the main steering file.

## 8. Loop Control Commands
Loop control is handled entirely through CLI commands:

- `add <file> [key=value|NUM]` — verifies the steering file, optionally replaces `@key@` and `@NUM@` macros in the document, and registers the loop with `TLoopManager`. This makes it easy to substitute run numbers or other parameters without editing the YAML.
- `resume`, `suspend`, `terminate` — operate on loop 0 by default, resuming, pausing, or terminating the event loop via `TLoopManager`. (Multiple loops can exist, but the default commands control the first one.)
- `help [command]` — lists all registered commands or detailed help for a specific one via `TCatCmdFactory`, so you can discover new utilities quickly.

Issue these commands at the `artemis` prompt in the order shown in the quick-start snippet.

## 9. Processor API & Extensibility
All processors derive from `art::TProcessor`, which provides lifecycle hooks (`Init`, `BeginOfRun`, `Process`, `PreLoop`, `PostLoop`, `Terminate`), control helpers (`SetStopLoop`, `SetEndOfRun`, etc.), and a state machine (`INIT`, `READY`, `ERROR`) so the loop can skip faulty processors. The base class exposes rich parameter-registration helpers for inputs, outputs, infos, and optional values, automatically wiring ROOT collections and arrays via the `IOCollection` helper. You can register parameters either directly or by passing `Parameter<T>` helpers, and you can register I/O collections that create `TClonesArray` branches on demand. When designing custom processors (often compiled into `libuser`), call `RegisterProcessorParameter`, `RegisterInputCollection`, and related helpers inside your constructor, then implement the relevant lifecycle hooks. Finally, load your shared library in `artemislogon.C` so the CLI can instantiate your processor types from YAML.

## 10. Interactive Utilities
Beyond loop control, ARTEMIS ships with several interactive helpers registered in `artemislogon.C`:

- `save [canvasName]` — saves the active ROOT canvas (or the named canvas) into a timestamped file tree, optionally adding date-based directories or file formats; useful for batch snapshots during online monitoring.
- `fls` — lists all currently open ROOT files, indicating which one is active so you can inspect contents quickly.
- `fcd <id>` — changes the current directory to the ROOT file identified by `fls`, simplifying navigation across multiple outputs.

Combine these with standard ROOT commands (e.g., `ls`, `cd`, drawing macros) to monitor your analysis interactively.

## 11. Example Workflow
1. **Install dependencies**: Ensure ROOT has Minuit2 and Geometry, and install YAML-CPP as a shared library. Build ARTEMIS via `autoreconf --install`, `./configure --with-yaml-cpp=<path>`, `make`, `make install`.
2. **Prepare steering and histogram definitions**: Copy `example/rndmEventStore.yaml` and `example/histdef.yaml` (or `example/ridfEventStore.yaml` for RIDF data) and tailor processor parameters such as input files, output collection names, or tree projection definitions.
3. **Launch ARTEMIS**: Run `artemis`, let `artemislogon.C` register your processors/commands, and confirm with `help`. Use the `add` command with optional macro replacements to queue your steering file; then `resume` to start processing, `suspend` to pause while tuning histograms, and `terminate` once satisfied.
4. **Monitor & save results**: Use histogram commands (e.g., `ht`, `hb`, registered in the logon script) and `save` to export canvases. Inspect output ROOT files with `fls` and `fcd`, and rely on the tree projection processor to fill histograms and write them out via `TCatCmdHstore` automatically at loop end.
5. **Extend as needed**: Implement custom processors by subclassing `TProcessor`, registering parameters/I/O, and loading them through `libuser` in `artemislogon.C`. Update your steering YAML to reference the new type and rerun the `add/resume` cycle.

With these steps you can iterate rapidly on analysis logic, instrument online monitoring, and maintain reproducible metadata for each ARTEMIS run.

## 12. Testing
- Not run (read-only review).
