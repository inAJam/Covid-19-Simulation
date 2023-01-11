# first run the C program to create the txt file to store the data for the infection then run this program

import matplotlib.pyplot as plt

# reading from the files and storing the data in the appropriate lists
with open('Covid19.txt') as f:
    lines = f.readlines()
    # list storing the days
    x = [float(line.split()[0]) for line in lines]

    # list storing the number of people susceptible
    s = [float(line.split()[1]) for line in lines]

    # list storing the number of people infected
    i = [float(line.split()[2]) for line in lines]

    # list storing the number of people recovered
    r = [float(line.split()[3]) for line in lines]

plt.plot(x, s, label='$Susceptible$', color='b')
plt.plot(x, i, label='$Infected$', color='r')
plt.plot(x, r, label='$Recovered$', color='black')

# naming the x axis
plt.xlabel('day')
# naming the y axis
plt.ylabel('Number of people')

# giving a title to the graph
plt.title('Infection curves')

plt.legend(loc='best')
plt.grid()
plt.show()
