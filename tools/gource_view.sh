# You need Gource to run this command.
# Get it here : https://code.google.com/p/gource/

# -s : seconds per day

# Displays an animation of a versionned project over time.
gource -s 0.2 -i 0 --hide filenames --max-user-speed 1000 --file-filter "\bexternal\b|\bassimp\b|\bfreetype\b"
