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

# Generate CSV file
N = 10
generate_random_csv('/Users/guna/Education/Masters/2ndSemester/COT6405_DAA/daa_project/Dataset/setA_'+str(N)+'.csv', N)
generate_random_csv('/Users/guna/Education/Masters/2ndSemester/COT6405_DAA/daa_project/Dataset/setB_'+str(N)+'.csv', N)
