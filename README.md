
This project is a hands-on workshop to learn how to handle C++-based microservices. It is a simple application that manages different endpoints for a messaging platform. The project's goals are as follows:

## Goals

### 1. **Authentication Management**
- **Registration:** Allows users to create an account on the application.
- **Login:** Allows users to log in to their account.
- **Logout:** Allows users to log out securely.
- **Session Validation:** Verifies if a user is authenticated and has an active session.

### 2. **Messaging**
- **Message Management:** Users can send, receive, and manage messages within the platform.

### 3. **Storage**
- **Use of MinIO:** Implements a MinIO container for storing images or files shared by users within the messaging platform.

### 4. **Database**
- **MongoDB:** The application uses MongoDB to store user information, messages, and other relevant data. Each microservice has its own MongoDB instance to ensure data isolation and scalability.

---

## Architecture

The application consists of multiple microservices, each with its own container. These microservices include:

- **Authentication:** Handles all aspects related to registration, login, logout, and session validation.
- **Messaging:** Manages the user messages.
- **Storage:** Handles the storage and management of files and images shared by users.
- **NGINX:** Acts as a reverse proxy to orchestrate user requests, directing them to the appropriate microservice.

Each microservice consists of:
- A service that handles business logic in C++.
- An independent MongoDB database for each microservice.

### Architecture Diagram

```
Client
   ↓
NGINX (Load Balancer/Reverse Proxy)
   ↓ ↓ ↓
 Auth Service ─ MongoDB (auth-db)
 Messaging Service ─ MongoDB (messaging-db)
 Storage Service ─ MongoDB (storage-db)
```

---

## Technologies

- **C++:** Main language for implementing the microservices.
- **MongoDB:** NoSQL database used to store user information, messages, and other data.
- **MinIO:** Container for storing images and shared files.
- **NGINX:** Reverse proxy used to direct user requests to the appropriate microservices.
- **Docker:** Used to containerize each microservice and its respective database.

---

## Usage Instructions

### 1. **Prerequisites**
- Docker and Docker Compose installed on your system.

### 2. **Clone the Repository**

```bash
git clone https://github.com/jgmr2/cchat.git
cd cchat
```

### 3. **Build and Start the Containers**

```bash
docker-compose up --build
```

This command will build the Docker images for the microservices and start the necessary containers for the application to work correctly. The NGINX service will handle load balancing and direct the requests to the corresponding services.

### 4. **Access the Application**
Once the containers are running, you can access the application through your browser at:

```
http://localhost
```

---

## Project Structure

```
/taller-microservicios-cpp
├── /auth                    # Authentication microservice
│   ├── /src
│   ├── /config
│   └── Dockerfile
├── /messaging               # Messaging microservice
│   ├── /src
│   ├── /config
│   └── Dockerfile
├── /storage                 # Storage microservice
│   ├── /src
│   ├── /config
│   └── Dockerfile
├── /nginx                   # NGINX reverse proxy configuration
│   ├── default.conf
│   ├── Dockerfile
│   ├── wait-for.sh          # Script to wait for services to be available
│   └── Dockerfile.nginx     # Custom Dockerfile for NGINX with wait script
├── /docs                    # Additional documentation
├── docker-compose.yml       # Container orchestration
└── README.md                # This file
```

---

## Contributing

If you want to contribute to this project, please follow these steps:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-new-feature`).
3. Make your changes and commit them (`git commit -am 'Add new feature'`).
4. Push to the branch (`git push origin feature-new-feature`).
5. Create a pull request.

---

Thank you for using this microservices workshop! If you have any questions or suggestions, feel free to open an issue on GitHub.