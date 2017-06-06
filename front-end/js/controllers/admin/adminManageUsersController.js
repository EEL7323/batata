angular.module("ruServer").controller("adminManageUsersCtrl", function ($scope, $http, $location, config, $rootScope) {

    $http.get(config.serverBaseUrl + "loadUsers.php")
        .then(function (response) {

            $scope.users = response.data;

        });

    $scope.users = [{
        name: "",
        registryNumber: "",
        tagNumber: 0,
        cellPhoneCredit: 0,
        cardCredit: 0,
        accessLevel: 1,
        checked: false       
    }];

    $scope.isChecked = function (users) {
        return users.some(function (user) {
            return user.checked;
        });
    };

    $scope.countChecked = function (users) {
        _acum = 0;
        users.forEach(function (user) {
            if (user.checked) _acum++;
        });
        return _acum;
    };

    $scope.deleteUsers = function (users) {
        var _token = localStorage['ruServer'];
        _accessLevel = _token.slice(7, 8);
        _sourceAdmin = _token.slice(19);
        _postData = {
            usersToDelete: [],
            sourceAdmin: _sourceAdmin,
            accessLevel: _accessLevel
        };
        users.forEach(function (user) {
            if (user.checked) _postData.usersToDelete.push(user.registryNumber);          
        });
        $http.post(config.serverBaseUrl + "deleteUsers.php", _postData)
            .then(function (response) {
                console.log(response);
            });
    };

    $scope.insertUser = function () {
        $location.path("/admin/manage-users/edit-user");
        $rootScope.selUser = {
            name: "",
            registryNumber: "",
            tagNumber: "",
            accessLevel: ""
        };
        $rootScope.editUserTitle = "Insert user info";
    };

    $scope.editUser = function (users) {
        $location.path("/admin/manage-users/edit-user");
        users.forEach(function (user) {
            if (user.checked) {
                $rootScope.selUser = {
                    name: user.name,
                    registryNumber: user.registryNumber,
                    tagNumber: (user.tagNumber * 1),
                    accessLevel: user.accessLevel
                };
                $rootScope.editUserTitle = "Edit user info";
            }
        });
    };

});