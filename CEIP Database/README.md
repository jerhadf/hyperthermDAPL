# To set up this database

### Install Azure Data Studio 
See install link here: https://learn.microsoft.com/en-us/sql/azure-data-studio/download-azure-data-studio?view=sql-server-ver16&culture=en-us&country=us&tabs=redhat-install%2Credhat-uninstall

Procedure: 

### Restoring a .bak file into a full SQL database with Docker on Mac
Here's a step-by-step guide to restore a .bak file into a full SQL database using Docker on your Mac:

Install Docker Desktop for Mac, if you haven't already. You can find the installation guide here: https://docs.docker.com/desktop/mac/install/

Pull the latest SQL Server image for Linux:

bash
Copy code
docker pull mcr.microsoft.com/mssql/server:2019-latest
Run the SQL Server Docker container with a custom name, exposing the default SQL Server port (1433), and setting the SA password:
arduino
Copy code
docker run -e 'ACCEPT_EULA=Y' -e 'SA_PASSWORD=<YourStrongPassword>' --name sql_server_container -p 1433:1433 -d mcr.microsoft.com/mssql/server:2019-latest
Replace <YourStrongPassword> with a strong password for the sa user.

Copy the .bak file into the Docker container:
bash
Copy code
docker cp /path/to/your/database.bak sql_server_container:/var/opt/mssql/backup/database.bak
Replace /path/to/your/database.bak with the actual path to your .bak file.

Connect to the SQL Server instance in the Docker container using the sqlcmd utility, which is included in the SQL Server image:
bash
Copy code
docker exec -it sql_server_container /opt/mssql-tools/bin/sqlcmd -S localhost -U sa -P <YourStrongPassword>
Replace <YourStrongPassword> with the password you set in step 3.

You are now connected to the SQL Server instance. Restore the .bak file by running the following commands in the sqlcmd prompt:
List the contents of the backup file:

css
Copy code
RESTORE FILELISTONLY FROM DISK = '/var/opt/mssql/backup/database.bak';
GO
Take note of the LogicalName values for the data and log files. You'll use them in the next step.

Restore the database:

sql
Copy code
RESTORE DATABASE YourDatabaseName
FROM DISK = '/var/opt/mssql/backup/database.bak'
WITH MOVE '<DataLogicalName>' TO '/var/opt/mssql/data/YourDatabaseName.mdf',
MOVE '<LogLogicalName>' TO '/var/opt/mssql/data/YourDatabaseName.ldf';
GO
Replace YourDatabaseName with the desired name for your database, and <DataLogicalName> and <LogLogicalName> with the respective logical names obtained in the previous step.

The database is now restored. Type exit to quit the sqlcmd utility.

To access the database, you can use a SQL client like Azure Data Studio. Download and install it from https://docs.microsoft.com/en-us/sql/azure-data-studio/download-azure-data-studio.

Open Azure Data Studio, click on "New Connection," and enter the following information:

Server: localhost,1433
Authentication type: SQL Login
User name: sa
Password: <YourStrongPassword>
Click "Connect"
Now you should be connected to your SQL Server instance in the Docker container, and you can see the restored database under the "Databases" folder.

Note that this setup is for local use only. If you want to share the database with others, you'll need to set up a SQL Server instance on a server or cloud platform and follow the steps in my previous response to allow other users to access it.

### Unpacking the .bak file into .csv files


### Setting up a shareable SQL server instance 
If you want to share the database with others on your GitHub repository, you can convert the database to a portable format like SQL scripts or CSV files. This way, users can download the files and import them into their own SQL Server instances. Here's how to do it:

Install Docker and run SQL Server as described in my previous response.

Restore the .bak file to a SQL Server instance running in a Docker container, also as described in my previous response.

Connect to the SQL Server instance using Azure Data Studio, as explained in the previous response.

Right-click on your database, choose "Tasks," and then select "Export Data-tier Application."

Save the .bacpac file to your local machine.

Extract the schema and data from the .bacpac file using the SqlPackage utility. Download and install SQL Server Data Tools (SSDT) for Visual Studio, which includes the SqlPackage utility: https://docs.microsoft.com/en-us/sql/ssdt/download-sql-server-data-tools-ssdt

Open the terminal (or Command Prompt on Windows) and navigate to the directory where the SqlPackage utility is installed (usually C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\Extensions\Microsoft\SQLDB\DAC\150 on Windows).

Run the following command to extract the schema and data from the .bacpac file:

bash
Copy code
./SqlPackage /a:Export /sf:/path/to/your/database.bacpac /tf:/path/to/your/output/folder/database.sql
Replace /path/to/your/database.bacpac with the path to your .bacpac file, and /path/to/your/output/folder with the path where you want to save the extracted SQL file.

Commit the generated SQL file to your GitHub repository.
Users who want to access the database can now clone your repository, import the SQL file into their own SQL Server instances, and run SQL commands on the imported data. To import the SQL file, they can use a tool like Azure Data Studio or SQL Server Management Studio.

Keep in mind that this approach shares a static snapshot of the database, and changes made by users will not be reflected in the original database. If you need to keep the data synchronized between users, you'll need to set up a central SQL Server instance and provide users with credentials to access it, as described in one of my previous responses.