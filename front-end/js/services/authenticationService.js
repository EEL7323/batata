angular.module("ruServer").service("authenticationService", function ($http, config) {

    var _checkToken = function (token) {
        var _data = { token: token };

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
                .catch(function (error) {
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
