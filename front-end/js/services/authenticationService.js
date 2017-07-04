// Service authentication service to check user's token and log him in
angular.module("ruServer").service("authenticationService", function ($http, config) {

    // Checks the token stored in local storage
    var _checkToken = function (token) {
        var _data = { token: token };

        // Post request to read if there is any user with the token given in post data
        var ret = $http.post(config.serverBaseUrl + "checkToken.php", _data)
                    .then(function (response) {
                        if (typeof (response.data) == "object") {
                            if (response.data.name != "") {
                                var _accessLevel = token.slice(7, 8);
                                return {
                                    error: "",
                                    access: true,
                                    accessLevel: _accessLevel,
                                    name: response.data.name,
                                    registry: response.data.registry_number,
                                    tag: response.data.tag_number
                                };
                            }
                            else {
                                return {
                                    error: response.data,
                                    access: false,
                                    accessLevel: 2,
                                    name: "",
                                    registry: "",
                                    tag: 0
                                };
                            }
                    }
                    else {
                        return {
                            error: response.data,
                            access: false,
                            accessLevel: 2,
                            name: "",
                            registry: "",
                            tag: 0
                        };
  
                    }
                
                })
                .catch(function (error) { // Error handling
                    return {
                        error: error,
                        access: false,
                        accessLevel: 2,
                        name: "",
                        registry: "",
                        tag: 0
                    };
                });

        return ret;
    };

    return {
        checkToken: _checkToken
    };

});
