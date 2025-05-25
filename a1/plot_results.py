import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

df = pd.read_csv("algos/SET9/a1/results.csv")

for dataset in df["dataset"].unique():
    subset = df[df["dataset"] == dataset]
    
    plt.figure(figsize=(12, 6))
    sns.lineplot(data=subset, x="size", y="time_ms", hue="algorithm", marker="o")
    plt.title(f"Sorting Time Comparison ({dataset})")
    plt.xlabel("Number of Strings")
    plt.ylabel("Time (ms)")
    plt.grid(True)
    plt.legend(title="Algorithm")
    plt.tight_layout()
    plt.savefig(f"algos/SET9/a1/time_{dataset}.png")
    plt.close()

    plt.figure(figsize=(12, 6))
    sns.lineplot(data=subset, x="size", y="comparisons", hue="algorithm", marker="o")
    plt.title(f"Character Comparisons ({dataset})")
    plt.xlabel("Number of Strings")
    plt.ylabel("Number of Character Comparisons")
    plt.grid(True)
    plt.legend(title="Algorithm")
    plt.tight_layout()
    plt.savefig(f"algos/SET9/a1/comparisons_{dataset}.png")
    plt.close()