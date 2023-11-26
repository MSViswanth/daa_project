import csv
import random

def generate_random_csv(filename, num_rows):
    with open(filename, 'w', newline='') as csvfile:
        fieldnames = ['Column_A', 'Column_B']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()

        for _ in range(num_rows):
            row = {
                'Column_A': round(random.uniform(0, 10), 2),
                'Column_B': round(random.uniform(0, 10), 2)
            }
            writer.writerow(row)

# Generate CSV file with 10 rows
generate_random_csv('/Dataset/setA_1000.csv', 1000)
generate_random_csv('/Dataset/setB_1000.csv', 1000)
