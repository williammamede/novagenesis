# Create the environment variables for the configuration
export NG_EMAIL="novagenesis@inatel.br"
export NG_EMAIL_PASSWORD=""

# Verify if the BASE variable is set if not set the default value
if [ -z "$BASE" ]; then
    export NG_BASE="~/workspace/novagenesis"
fi