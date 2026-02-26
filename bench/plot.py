# /// script
# requires-python = ">=3.11"
# dependencies = [
#   "matplotlib",
#   "numpy",
# ]
# ///

import argparse
import json
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

tab = {
    "BM_BuildMaxtree": "BuildTree",
    "BM_ComputeArea": "ComputeArea",
    "BM_Filter": "Filter",
}

def load_benchmarks(path):
    with open(path) as f:
        data = json.load(f)

    results = {}
    for bench in data["benchmarks"]:
        name = bench["name"]
        time_ns = bench["real_time"]
        unit = bench.get("time_unit", "ns")

        if unit == "ns":
            time_ms = time_ns / 1e6
        elif unit == "us":
            time_ms = time_ns / 1e3
        elif unit == "ms":
            time_ms = time_ns
        elif unit == "s":
            time_ms = time_ns * 1e3
        else:
            raise Exception(f"Unknown time unit: {unit}")

        results[name] = time_ms

    return results


def plot_comparison(cpu_results, gpu_results, output_dir):
    common = cpu_results.keys()

    cpu_times = [cpu_results[name] for name in common]
    gpu_times = [gpu_results[name] for name in common]

    x = np.arange(len(common))
    width = 0.3

    fig, ax = plt.subplots(figsize=(max(8, len(common) * 1.5), 5))

    bars_cpu = ax.bar(x - width / 2, cpu_times, width, label="CPU", color="orange")
    bars_gpu = ax.bar(x + width / 2, gpu_times, width, label="GPU", color="green")

    for bar in bars_cpu + bars_gpu:
        h = bar.get_height()
        ax.text(
            bar.get_x() + bar.get_width() / 2,
            h * 1.01,
            f"{h:.2f}",
            ha="center", va="bottom", fontsize=8
        )

    ax.set_xlabel("Pipeline Step")
    ax.set_ylabel("Time (ms)")
    ax.set_title("CPU vs GPU Benchmark")
    ax.set_xticks(x)
    ax.set_xticklabels([tab[elt] for elt in common], rotation=15, ha="right")
    ax.legend()
    ax.grid(axis="y", linestyle="-", alpha=0.5)

    fig.tight_layout()
    out_path = output_dir / "cpu_vs_gpu.png"
    fig.savefig(out_path, dpi=150)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("cpu_json", help="Path to CPU benchmark JSON file")
    parser.add_argument("gpu_json", help="Path to GPU benchmark JSON file")
    args = parser.parse_args()

    output_dir = Path("bench/plots")
    output_dir.mkdir(parents=True, exist_ok=True)

    cpu_results = load_benchmarks(args.cpu_json)
    gpu_results = load_benchmarks(args.gpu_json)

    plot_comparison(cpu_results, gpu_results, output_dir)


if __name__ == "__main__":
    main()