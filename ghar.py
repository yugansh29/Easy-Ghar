import tkinter as tk
from tkinter import ttk

# Function to update the status of the appliances
def toggle_appliance(appliance):
    current_state = appliances[appliance]['status']
    new_state = "Off" if current_state == "On" else "On"
    appliances[appliance]['status'] = new_state
    update_status()

def update_status():
    for appliance in appliances:
        appliances[appliance]['label'].config(text=f"{appliance} is {appliances[appliance]['status']}")

def update_temperature(value):
    temperature_label.config(text=f"Room Temperature: {value}°C")

# Setting up the main application window
root = tk.Tk()
root.title("Home Automation Simulation")
root.geometry("600x400")

# Dictionary to hold appliance status and labels
appliances = {
    "TubeLight": {"status": "Off", "label": None},
    "Fan": {"status": "Off", "label": None},
    "AC": {"status": "Off", "label": None},
    "Television": {"status": "Off", "label": None}
}

# Load appliance images
tubelight_image = tk.PhotoImage(file="tubelight.png")
fan_image = tk.PhotoImage(file="fan.png")
ac_image = tk.PhotoImage(file="ac.png")
tv_image = tk.PhotoImage(file="tv.png")

# Pictorial representations and buttons
appliance_images = {
    "TubeLight": tubelight_image,
    "Fan": fan_image,
    "AC": ac_image,
    "Television": tv_image
}

for idx, appliance in enumerate(appliances):
    # Image label
    image_label = tk.Label(root, image=appliance_images[appliance])
    image_label.grid(row=0, column=idx, padx=10, pady=10)

    # Status label
    status_label = tk.Label(root, text=f"{appliance} is Off")
    status_label.grid(row=1, column=idx, pady=5)
    appliances[appliance]['label'] = status_label

    # Toggle button
    toggle_button = tk.Button(root, text=f"Toggle {appliance}", command=lambda a=appliance: toggle_appliance(a))
    toggle_button.grid(row=2, column=idx, pady=5)

# Room temperature control
temperature_label = tk.Label(root, text="Room Temperature: 24°C")
temperature_label.grid(row=3, columnspan=4, pady=20)

temperature_slider = tk.Scale(root, from_=16, to=30, orient=tk.HORIZONTAL, command=update_temperature)
temperature_slider.set(24)
temperature_slider.grid(row=4, columnspan=4, padx=10)

# Start the Tkinter event loop
root.mainloop()
