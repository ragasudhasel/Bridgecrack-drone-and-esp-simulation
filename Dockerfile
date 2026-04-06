# Use official Python lightweight image
FROM python:3.10-slim

# Install system dependencies required for OpenCV
RUN apt-get update && apt-get install -y \
    libgl1-mesa-glx \
    libglib2.0-0 \
 && rm -rf /var/lib/apt/lists/*

# Set up a new user 'user' with user ID 1000 for Hugging Face spaces
RUN useradd -m -u 1000 user

# Switch to the 'user' user
USER user
ENV HOME=/home/user \
    PATH=/home/user/.local/bin:$PATH

# Set the working directory to the user's home directory
WORKDIR $HOME/app

# Copy requirements and install them securely
COPY --chown=user requirements.txt .
RUN pip install --no-cache-dir --upgrade pip
RUN pip install --no-cache-dir -r requirements.txt

# Copy the rest of the application files securely
COPY --chown=user . .

# Expose the standard Hugging Face port
EXPOSE 7860

# Command to run the application securely
CMD ["python", "web_app/app.py"]
