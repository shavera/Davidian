import davidian_client


def main():
    davidian_client.dummyFn()
    client = davidian_client.DavidianClient()
    client.load_file("TestSystem")



if __name__ == "__main__":
    main()
