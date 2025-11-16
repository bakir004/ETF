cd(@__DIR__)
ENV["GKSwstype"] = "100"

using Plots
gr()  # explicitly use GR backend

# Make a simple sine plot
x = 0:0.1:10
y = sin.(x)

p = plot(x, y, label = "sin(x)", xlabel = "x", ylabel = "y", title = "Test Plot")
savefig(p, "test_plot.png")
